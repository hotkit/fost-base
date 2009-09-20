/** \defgroup fost_core_settings settings
    \ingroup fost_core

    \brief A settings database used for software configuration.

    The settings library is intended to allow for program configuration in a sane manner. It features a thread safe database of setting values that can be queried in a type safe manner with defaults that are embedded in the code near their point of use. Settings can then be overriden at higher levels of code, with the expectation that the top layer application code will load configuration files that are able to change its software configuration.

    Settings themselves are \ref fost_core_json "JSON" objects, arrays or atoms (i.e. strings, numbers etc.).

    \section about About the settings database

    Settings are referenced by a compound key made up of two parts, the section and the name. For each setting and name there is a stack of setting values with later defined ones at the top and earlier defined ones at the bottom. When a setting is read the value at the top of the stack is returned. This means that default settings (bottom of the stack) can be placed in the source code and the application can load ever more specific configuration files. For example:

    - The base defaults from the loaded application are present.
    - Read a general machine specific settings file (i.e. /etc/fost.ini or \%windir\%\\fost.ini).
    - Read an application specific settings file for that machine (i.e. /etc/app.ini or \%appdir\%\\app.ini). -- <b>not implemented</b>
    - Read a user specific settings file (i.e. ~/.app.ini or \%home\%\\app.ini). -- <b>not implemented</b>
    - Read settings specified on the command line.

    This way the user retains complete control over the settings used when a program is run, but common settings can still be abstracted away from the user and the given application.

    Internally settings are stored using Fost's \ref fost_core_json "JSON" library. Within \ref fost_cli "fost-cli" is an \ref fost_cli_ini "INI file parser" which is able to handle a format familiar to most system administrators.
*/
/** \class fostlib::setting settings.hpp fost/core
    \ingroup fost_core_settings
    \param V The type in the settings database. This must be one of the types that is storable in fostlib::json instances.

    Instances of this class represent individual settings in the settings database.

    Reading a setting can be done in one of two ways:

    <pre class="language-cpp">
    const fostlib::setting< string > c_setting( L"An example", L"Examples", L"Hello", "Hello world!", true );

    // Returns the current value, not the default
    c_setting.value();

    // Returns the same value
    fostlib::setting< string >::value( L"Examples", L"Hello" );
    </pre>

    <h2>Examples</h2>

    <pre class="language-cpp">
    #include <iostream>
    #include <fost/core>

    namespace {
        const fostlib::setting< string > c_hello( fostlib::string( __FILE__ ), L"Examples", "Hello", "Hello", true );
    }

    int main() {
        std::cout << c_hello.value() << std::endl;
        fostlib::setting< string > world( L"main()", L"Examples", L"Hello", "Hello world!" );
        std::cout << c_hello.value() << std::endl;
        return 0;
    }
    </pre>

    This will print:

    <pre>
    Hello
    Hello world!
    </pre>

    Howver, if the following INI file is loaded in at the command line:

    <pre class="language-ini">
    [Examples]
    hello=Goodbye
    </pre>

    Then the output will change to the below.

    <pre>
    Goodbye
    Hello world!
    </pre>

    This is because the value in the INI file will override the default defined in the source code, but the new value defined inside main will override the value in the INI file.
*/
/** \fn fostlib::setting< type >::setting (const string &domain, const string &section, const string &name, const t_final_value &value, bool def=false)
    \param domain The file or origin of this value within the settings database. Normally you will want to use the source file name.
    \param section The section name. This is the first part of the key used to fetch a setting.
    \param name The name of the value within the section. The second half of the key used to fetch a setting.
    \param value The value that is being specified.
    \param def True if this is the default value for the setting. If you are defining a new settings you should always set def to true. If you are overriding a setting value then this should be false.

    \brief Constructor used to create a new setting, or override one where only the section and name are known.

    Getting the value of def correct is very important. For the new settings which are also defining a default value (the one which is to be at the bottom of the settings stack) this should be set to true. For any other setting (i.e. one that actually wishes to set a configuration setting) this must be false so that it actually overrides less important settings.
*/
/** \fn fostlib::setting< type >::setting (const string &domain, const setting< t_final_value > &setting, const t_final_value &value)
    \param domain The file or origin of this value within the settings database. Normally you will want to use the source file name.
    \param setting A setting instance that represents the setting which is to get a new value.
    \param value The value that is being specified.

    \brief Constructor used to override a setting where a defining setting object is avilable.
*/


/** \class fostlib::exceptions::missing_setting fost/exception/missing_setting.hpp
    \ingroup fost_core_settings

    \brief Thrown when a requsted setting cannot be found in the settings database.
*/
/** \class fostlib::exceptions::settings_fault fost/exception/missing_setting.hpp
    \ingroup fost_core_settings

    \brief This exception is not used.
*/

