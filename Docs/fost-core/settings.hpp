/** \defgroup fost_core_settings settings
    \ingroup fost_core

    The settings library is intended to allow for program configuration in a sane manner. It features a thread safe database of setting values that can be queried in a type safe manner with defaults that are embedded in the code near their point of use. Settings can then be overriden at higher levels of code, with the expectation that the top layer application code will load configuration files that are able to change its software configuration.

    Settings themselves are \ref fost_core_json "JSON" objects, arrays or atoms (i.e. strings, numbers etc.).

    <h2>About the settings database</h2>

    Settings are referenced by a compound key made up of two parts, the section and the name. For each setting and name there is a stack of setting values with later defined ones at the top and earlier defined ones at the bottom. When a setting is read the value at the top of the stack is returned. This means that default settings (bottom of the stack) can be placed in the source code and the application can load ever more specific configuration files. For example:

    - The base defaults from the loaded application are present.
    - Read a general machine specific settings file (i.e. /etc/fost.ini or \%windir\%\\fost.ini).
    - Read an application specific settings file for that machine (i.e. /etc/app.ini or \%appdir\%\\app.ini). -- <b>not implemented</b>
    - Read a user specific settings file (i.e. ~/.app.ini or \%home\%\\app.ini). -- <b>not implemented</b>
    - Read settings specified on the command line.

    This way the user retains complete control over the settings used when a program is run, but common settings can still be abstracted away from the user and the given application.

    Internally settings are stored using Fost's \ref fost_core_json "JSON" library. Within \ref fost_cli "fost-cli" is an \ref fost_cli_ini "INI file parser" which is able to handle a format familiar to most system administrators.

    <h2>Using settings</h2>

    Normally the code will want to create a default value for a given setting using the following constructor (see example for working code):

    <pre class="language-cpp">
    fostlib::setting&lt; type &gt;::setting(
        const fostlib::string &amp;source_location,
        const fostlib::string &amp;section,
        const fostlib::string &amp;name,
        const type &amp;value,
        bool is_default
    );
    </pre>

    - type -- The type of the setting. This is the type that will pulled out from ''value'' when the setting is to be read.
    - source_location -- The file or origin of this value within the settings database. Normally you will want to use the source file name.
    - section -- The section name. This is the first part of the key used to fetch a setting.
    - name -- The name of the value within the section. The second half of the key used to fetch a setting.
    - value -- The value that is being specified.
    - is_default -- For the default (the one which is to be at the bottom of the settings stack) this should be set to `true`. For any other setting (i.e. one that actually wishes to set a configuration setting) this must be `false` so that it actually overrides less important settings.

    Within an INI file the section and the value and name correspond to the following:

    <pre class="language-ini">
    [section]
    name=value
    </pre>
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
*/
