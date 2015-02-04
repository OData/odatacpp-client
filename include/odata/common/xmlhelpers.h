//---------------------------------------------------------------------
// <copyright file="xmlhelpers.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once
#ifndef _XMLHELPERS_H
#define _XMLHELPERS_H
#ifdef WIN32
#include <atlbase.h>
#include <xmllite.h>
#else
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <stack>
#endif 

#include <string>
#include "cpprest/basic_types.h"
#include "cpprest/streams.h"
#include "cpprest/containerstream.h"

#include "odata/common/platform.h"

namespace odata { namespace common {

/// <summary>
/// XML reader based on xmlllite in Windows and libxml2 in other platform.
/// </summary>
class xml_reader
{
public:

    virtual ~xml_reader() {}

    /// <summary>
    /// Parse the given xml string/stream. Returns true if it finished parsing the stream to the end, and false
    /// if it was asked to exit early via pause()
    /// </summary>
    bool parse();

protected:

    xml_reader() : m_continueParsing(true), m_streamDone(false)
    {
    }

    xml_reader(concurrency::streams::istream stream) : m_continueParsing(true), m_streamDone(false)
    {
        initialize(stream);
    }

    /// <summary>
    /// Callback for handling the start of an element.
    /// </summary>
    virtual void handle_begin_element(const ::utility::string_t&)
    {
    }

    /// <summary>
    /// Callback for handling the element text.
    /// </summary>
    virtual void handle_element(const ::utility::string_t& )
    {
    }

    /// <summary>
    /// Callback for handling the end of an element.
    /// </summary>
    virtual void handle_end_element(const ::utility::string_t& )
    {
    }

    /// <summary>
    /// Logs an error from processing XML
    /// </summary>
    virtual void log_error_message(const ::utility::string_t& message, unsigned long error = 0)
    {
        UNREFERENCED_PARAMETER(message);
        UNREFERENCED_PARAMETER(error);
    }

    /// <summary>
    /// Returns the parent element name
    /// </summary>
    ::utility::string_t get_parent_element_name(size_t pos = 0);

    /// <summary>
    /// Returns the current element name
    /// </summary>
    ::utility::string_t get_current_element_name();

    /// <summary>
    /// Returns the current element name with the prefix if any. 
    /// </summary>
    ::utility::string_t get_current_element_name_with_prefix();

    /// <summary>
    /// Returns the current element value
    /// </summary>
    ::utility::string_t get_current_element_text();

    /// <summary>
    /// Moves to the first attribute in the node
    /// </summary>
    bool move_to_first_attribute();

    /// <summary>
    /// Moves to the first attribute in the node
    /// </summary>
    bool move_to_next_attribute();

    /// <summary>
    /// Extracts the current element value into the provided type
    /// </summary>
    template <class T>
    void extract_current_element(T& value)
    {
        ::utility::istringstream_t iss(get_current_element_text());
        iss >> value;
    }

    /// <summary>
    /// Initialize the reader
    /// </summary>
    ODATACPP_CLIENT_API void initialize(concurrency::streams::istream stream);

    /// <summary>
    /// Remove Byte Order Mark from the stream
    /// </summary>
    void remove_bom(concurrency::streams::istream stream);

    /// <summary>
    /// Can be called by the derived classes in the handle_* routines, to cause the parse routine to exit early,
    /// in order to capture records as they are parsed. Parsing is resumed by invoking the parse method again.
    /// </summary>
    void pause() { m_continueParsing = false; }

#ifdef WIN32
    CComPtr<IXmlReader> m_reader;
#else
    xmlTextReaderPtr m_reader;
    std::string m_data;
#endif 

    std::vector<::utility::string_t> m_elementStack;
    bool m_continueParsing;
    bool m_streamDone;
};

}} // namespace odata::edm

#endif
