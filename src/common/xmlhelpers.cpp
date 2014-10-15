/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "odata/common/xmlhelpers.h"

#ifdef WIN32
#include "odata/common/xmlstream.h"
#else
typedef int XmlNodeType;
#define XmlNodeType_Element XML_READER_TYPE_ELEMENT
#define XmlNodeType_Text XML_READER_TYPE_TEXT
#define XmlNodeType_EndElement XML_READER_TYPE_END_ELEMENT
#endif

using namespace utility;
using namespace concurrency;

namespace odata { namespace common {

    void xml_reader::initialize(streams::istream stream)
    {
        remove_bom(stream);

#ifdef WIN32
        HRESULT hr;
        CComPtr<IStream> pInputStream;
        pInputStream.Attach(xmlstring_istream::create(stream));

        if (pInputStream == nullptr)
        {
            auto error = ERROR_NOT_ENOUGH_MEMORY;
            log_error_message(U("XML reader IStream creation failed"), error);
            throw utility::details::create_system_error(error);
        }

        if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&m_reader, NULL)))
        {

            auto error = GetLastError();
            log_error_message(U("XML reader CreateXmlReader failed"), error);
            throw utility::details::create_system_error(error);
        }

        if (FAILED(hr = m_reader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
        {

            auto error = GetLastError();
            log_error_message(U("XML reader SetProperty failed"), error);
            throw utility::details::create_system_error(error);
        }

        if (FAILED(hr = m_reader->SetInput(pInputStream)))
        {

            auto error = GetLastError();
            log_error_message(U("XML reader SetInput failed"), error);
            throw utility::details::create_system_error(error);
        }
#else
        concurrency::streams::stringstreambuf buffer;
        stream.read_to_end(buffer).get();
        m_data = buffer.collection();
        const char* xmlBuffer = reinterpret_cast<const char*>(m_data.data());
        unsigned int size = static_cast<unsigned int>(m_data.size());
        xmlInitParser();
        m_reader = xmlReaderForMemory(xmlBuffer, size, "", "", XML_PARSE_RECOVER);
#endif
    }

    /// <summary>
    /// Remove Byte Order Mark from the stream
    /// </summary>
    void xml_reader::remove_bom(streams::istream stream)
    {
        // Synchronous.
        if (stream.peek().get() == 0xEF
            && stream.peek().get() == 0xBB
            && stream.peek().get() == 0xBF)
        {
            stream.read().get();
            stream.read().get();
            stream.read().get();
            return;
        }
    }

    bool xml_reader::parse()
    {
        if (m_streamDone) return false;
        // Set this to true each time the parse routine is invoked. Most derived readers will only invoke parse once.
        m_continueParsing = true;

        // read until there are no more nodes
#ifdef WIN32
        HRESULT hr;
        XmlNodeType nodeType;
        while (m_continueParsing && S_OK == (hr = m_reader->Read(&nodeType)))
        {
#else
        if (m_reader == nullptr)
            return !m_continueParsing; // no XML document to read

        while (m_continueParsing && xmlTextReaderRead(m_reader))
        {
            auto nodeType = xmlTextReaderNodeType(m_reader);
#endif
            switch (nodeType)
            {

            case XmlNodeType_Element:
            {
                auto name = get_current_element_name();
#ifdef WIN32
				BOOL is_empty_element = m_reader->IsEmptyElement();
#else
                bool is_empty_element = xmlTextReaderIsEmptyElement(m_reader);
#endif
                m_elementStack.push_back(name);
                handle_begin_element(name);

                if (is_empty_element)
                {
                    handle_end_element(name);
                    m_elementStack.pop_back();
                }
            }
                break;

            case XmlNodeType_Text:
                handle_element(m_elementStack.back());
                break;

            case XmlNodeType_EndElement:
                handle_end_element(m_elementStack.back());
                m_elementStack.pop_back();
                break;

            default:
                break;
            }
        }

        // If the loop was terminated because there was no more to read from the stream, set m_streamDone to true, so exit early
        // the next time parse is invoked.
        if (m_continueParsing) m_streamDone = true;
        // Return false if the end of the stream was reached and true if parsing was paused. The return value indicates whether
        // parsing can be resumed.
        return !m_continueParsing;
    }

    ::utility::string_t xml_reader::get_parent_element_name(size_t pos)
    {
        if (m_elementStack.size() > pos + 1)
        {
            size_t currentDepth = m_elementStack.size() - 1;
            size_t parentDepth = currentDepth - 1;

            if (pos <= parentDepth)
            {
                return m_elementStack[parentDepth - pos];
            }
        }

        // return empty string
        return ::utility::string_t();
    }

    ::utility::string_t xml_reader::get_current_element_name()
    {
#ifdef WIN32
        HRESULT hr;
        const wchar_t * pwszLocalName = NULL;

        if (FAILED(hr = m_reader->GetLocalName(&pwszLocalName, NULL)))
        {
            auto error = GetLastError();
            log_error_message(U("XML reader GetLocalName failed"), error);
            throw utility::details::create_system_error(error);
        }
        return ::utility::string_t(pwszLocalName);
#else
        xmlChar* valueXml = xmlTextReaderLocalName(m_reader);
        std::string input((char*)valueXml);
        ::utility::string_t stringt = ::utility::conversions::to_string_t(input);
        xmlFree(valueXml);
        return stringt;
#endif
    }

    ::utility::string_t xml_reader::get_current_element_name_with_prefix()
    {
#ifdef WIN32
        HRESULT hr;
        const wchar_t * pwszName = NULL;

        if (FAILED(hr = m_reader->GetQualifiedName(&pwszName, NULL)))
        {
            auto error = GetLastError();
            log_error_message(U("XML reader GetQualified failed"), error);
            throw utility::details::create_system_error(error);
        }
        return ::utility::string_t(pwszName);
#else
        throw std::runtime_error("Not implemented");
#endif
    }

    ::utility::string_t xml_reader::get_current_element_text()
    {
#ifdef WIN32
        HRESULT hr;
        const wchar_t * pwszValue;

        if (FAILED(hr = m_reader->GetValue(&pwszValue, NULL)))
        {
            auto error = GetLastError();
            log_error_message(U("XML reader GetValue failed"), error);
            throw utility::details::create_system_error(error);
        }

        return ::utility::string_t(pwszValue);
#else
        xmlChar* valueXml = xmlTextReaderValue(m_reader);
        std::string input((char*)valueXml);
        ::utility::string_t stringt = ::utility::conversions::to_string_t(input);
        xmlFree(valueXml);
        return stringt;
#endif
    }

    bool xml_reader::move_to_first_attribute()
    {
#ifdef WIN32
        HRESULT hr;
        if (FAILED(hr = m_reader->MoveToFirstAttribute()))
        {
            auto error = GetLastError();
            log_error_message(U("XML reader MoveToFirstAttribute failed"), error);
            throw utility::details::create_system_error(error);
        }
        return (hr == S_OK);
#else
        return xmlTextReaderMoveToFirstAttribute(m_reader) > 0;
#endif
    }

    bool xml_reader::move_to_next_attribute()
    {
#ifdef WIN32
        HRESULT hr;
        if (FAILED(hr = m_reader->MoveToNextAttribute()))
        {
            auto error = GetLastError();
            log_error_message(U("XML reader MoveToNextAttribute failed"), error);
            throw utility::details::create_system_error(error);
        }
        return (hr == S_OK);
#else
        return xmlTextReaderMoveToNextAttribute(m_reader) > 0;
#endif
    }
}}
// namespace odata::edm
