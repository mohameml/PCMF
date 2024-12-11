# Instructions on Windows machines

- **Prerequisite**: if necessary, download and install *vcpkg* from github, based on the  [following instructions](https://github.com/microsoft/vcpkg#quick-start-windows).
- Use *vcpkg* to install nlohmann-json (64 bit version): `.\vcpkg.exe install nlohmann-json:x64-windows`

- Open *cmake-gui.exe*.
- Select the *BarrierOptionPricing* folder for the location of the source code.
- Select the Build folder of your choice.
- Add a 'CMAKE_PREFIX_PATH' entry, set its type to 'PATH' and provide the location to libpnl.
- Add a 'CMAKE_TOOLCHAIN_FILE' entry, set its type to 'FILEPATH' and provide the location to the file *[vcpkg root]/scripts/buildsystems/vcpkg.cmake*
- Click on 'Configure', then 'Generate'.

  **Warning:** CMake needs to download googletests, which can take a lot of time

  **Nb:** if the unit tests do not compile, there is a commented line that can be modified in the corresponding CMakeLists file.

- The unit tests can be run either from Visual Studio, or from the `Build\BarrierOptionPricing_test\Debug` (or `\Release`) folder.
- The pricer can be found in the `Build\SourceCode\BarrierOptionPricing\Debug` (or `\Release`) folder. It takes a .json file as an argument, one such file is provided in the BarrierFiles folder.