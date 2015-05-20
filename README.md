# Welcome to odatacpp_client
odatacpp_client is an open-source C++ library that implements the Open Data Protocol (OData). It supports the [OData protocol version 4.0](http://docs.oasis-open.org/odata/odata/v4.0/os/part1-protocol/odata-v4.0-os-part1-protocol.html). It currently serves only client and client side proxy generation (code gen) aspects of OData. The server library is under development and will be included soon.

# Getting started

## Getting the source

    git clone https://github.com/odata/odatacpp-client

## Building and testing

Currently the following target platforms are supported:

  * Windows 32-bit
  * OS X 64-bit
  * iOS

### Building on Windows with Visual Studio 2012

1.Please ensure that you have Visual Studio 2012 installed.

2.ODataCpp depends on Casablanca (http://casablanca.codeplex.com) which is distributed as a NuGet package.
  Please restore this NuGet package before you open any solution:

    cd odatacpp_client\msvc
    .nuget\nuget.exe restore odatacpp.sln

3.Open 'odatacpp.sln' under 'odatacpp_client\msvc' with VS2012 and click 'Build Solution' in the 'BUILD' menu.

4.Built libraries are placed under 'odatacpp_client\output\\\<Configuration\>' where '\<Configuration\>' could be either 'Debug' or 'Release' according to your build configuration in VS2012.

### Building on Windows with MSBuild

1.Setup build environment for VS2012:

    cd odatacpp_client
    powershell
    .\setup_ps_env_VS2012.ps1

   If you receive an error message like 'running scripts is disabled on this system', please run PowerShell as administrator, type the following command, and then rerun the setup script above.

    Set-ExecutionPolicy RemoteSigned

2.Here are some examples to invoke MSBuild:

   1) Build Debug version of odatacpp_client libraries in parallel.

    msbuild /m

   2) Build Release version of odatacpp_client libraries.

    msbuild /p:Configuration=Release

   3) Rebuild Debug version of odatacpp_client.

    msbuild /t:Rebuild /p:Configuration=Debug

   4) Clean build outputs.

    msbuild /t:Clean

3.Built libraries are placed under the same folder of VS2012. Actually they are no different than the ones built by VS2012.

### Running tests on Windows

1.After you have successfully built the libraries, you can run our functional and end-to-end tests to check the basic functionality.

  1) Test the Debug version of odatacpp_client Libraries:

    cd odatacpp_client\output\Debug
    TestRunner.exe odata_functional_test.vs11d.dll odata_e2e_test.vs11d.dll

  2) Test the Release version of odatacpp_client Libraries:

    cd odatacpp_client\output\Release
    TestRunner.exe odata_functional_test.vs11.dll odata_e2e_test.vs11.dll

  Don't worry about the failed tests as they are known issues and we will get them fixed in the future releases.

### Building on OS X

1.Please ensure that you have OS X later than 10.9, Xcode later than 5.0 and Xcode Command Line Tools installed.

2.Install the Homebrew package manager (http://brew.sh). Skip this step if you want to use your own package manager.

3.Install the required packages to build odatacpp_client via Homebrew or your own package manager.

    brew install cmake git boost pkg-config

4.Clone (do not change the location below) and build the dependency library Casablanca v2.1.0 (http://casablanca.codeplex.com).

    cd odatacpp_client
    mkdir lib
    cd lib
    git clone https://git01.codeplex.com/casablanca
    cd casablanca
    git checkout v2.1.0
    mkdir build.release
    cd build.release
    cmake ../Release -DCMAKE_BUILD_TYPE=Release
    make -j 4

5.Return to the root folder of odatacpp_client and generate 'Makefile' using CMake.

    cd ../../..
    cmake -DCMAKE_BUILD_TYPE=Debug # replace 'Debug' with 'Release' if needed
    make -j 4

  Ignore the possible warning CMP0042 issued by CMake.

6.Please find your built libraries under 'output'.

### Running tests on OS X

After successfully building the libraries, you can run the functional and end-to-end tests via the terminal.

    cd odatacpp_client/output
    ./test_runner *tests*

Please ignore the failed tests as they are already known to us and will be fixed in the future releases.

### Building for iOS

Complete on Building on OS X. Return to the root folder of odatacpp_client, run following commands.


    cd lib/casablanca/Build_iOS
    ./configure.sh
    cd ../../..
    cd ios
    ./configure.sh

The libraries (libodata-client.a and libodata-library.a) will be built in /ios/build.ios.

# Documentation
Please refer to our [wiki](https://github.com/OData/odatacpp_client/wiki) for documentation.

# Community
## Contribution
Please refer to the [CONTRIBUTION.md](https://github.com/OData/odatacpp_client/blob/master/CONTRIBUTING.md).
## Issue tracker
To report bugs and require features, please use our [issue tracker](https://github.com/OData/odatacpp_client/issues?state=open).

## Team blog
Please visit http://blogs.msdn.com/b/odatateam/.
