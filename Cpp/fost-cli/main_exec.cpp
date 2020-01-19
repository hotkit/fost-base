/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-cli.hpp"
#include <fost/main.hpp>


fostlib::loaded_settings::loaded_settings(
        const fostlib::string &name, const fostlib::string &banner)
: name(name),
  banner(banner),
  c_ini_file("fost-cli/main.cpp", name, "IniFile", name + ".ini", true),
  c_json_file("fost-cli/main.cpp", name, "JSON File", name + ".json", true),
  c_banner("fost-cli/main.cpp", name, "Banner", true, true),
  c_settings("fost-cli/main.cpp", name, "Settings", false, true),
  c_logging(
          "fost-cli/main.cpp",
          name,
          "Logging sinks",
          []() {
              fostlib::json ret, sink;
              fostlib::insert(sink, "name", "stdout");
              fostlib::insert(sink, "configuration", fostlib::json::object_t{});
              fostlib::push_back(ret, "sinks", sink);
              return ret;
          }(),
          true) {}


void fostlib::standard_arguments(
        const loaded_settings &settings, ostream &out, arguments &args) {
    args.commandSwitch("b", settings.name, "Banner");
    if (settings.c_banner.value()) out << settings.banner << std::endl;

    args.commandSwitch("s", settings.name, "Settings");
    if (settings.c_settings.value()) setting<json>::printAllOn(out);
}


namespace {
    int exception_wrapper(fostlib::ostream &, std::function<int(void)> f) {
        try {
            fostlib::exceptions::structured_handler handler;
#ifdef WIN32
            fostlib::ini_file base_settings_file(
                    fostlib::arguments::environment("windir") + "\\fost.ini");
#else
            fostlib::ini_file base_settings_file("/etc/fost.conf");
#endif

            return f();
        } catch (fostlib::exceptions::exception &e) {
            std::cerr << std::endl
                      << "Caught a fostlib::exceptions::exception\n"
                      << e << std::endl;
        } catch (std::exception &e) {
            std::cerr << std::endl
                      << "Caught a std::exception\n"
                      << fostlib::string(e.what()) << std::endl;
        } catch (...) {
            std::cerr << "\nUnknown exception type caught" << std::endl;
        }
        return 2;
    }

    int simple_wrapper(
            const fostlib::loaded_settings &settings,
            fostlib::ostream &out,
            fostlib::arguments &args,
            int (*main_f)(fostlib::ostream &, fostlib::arguments &)) {
        args.commandSwitch("i", settings.c_ini_file);
        fostlib::ini_file ifile(settings.c_ini_file.value());
        args.commandSwitch("j", settings.name, "JSON File");
        fostlib::settings jfile(settings.c_json_file);
        fostlib::standard_arguments(settings, out, args);
        fostlib::log::global_sink_configuration log_sinks(
                settings.c_logging.value());
        return main_f(out, args);
    }
    int complex_wrapper(
            const fostlib::loaded_settings &settings,
            fostlib::ostream &out,
            fostlib::arguments &args,
            int (*main_f)(
                    const fostlib::loaded_settings &,
                    fostlib::ostream &,
                    fostlib::arguments &)) {
        return main_f(settings, out, args);
    }
}


int fostlib::main_exec(
        const loaded_settings &settings,
        ostream &out,
        arguments &args,
        int (*main_f)(fostlib::ostream &, fostlib::arguments &)) {
    return exception_wrapper(
            out, [&]() { return simple_wrapper(settings, out, args, main_f); });
}


int fostlib::main_exec(
        const loaded_settings &settings,
        ostream &out,
        arguments &args,
        int (*main_f)(
                const fostlib::loaded_settings &,
                fostlib::ostream &,
                fostlib::arguments &)) {
    return exception_wrapper(out, [&]() {
        return complex_wrapper(settings, out, args, main_f);
    });
}
