/*
    Copyright 2008-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/main.hpp>


fostlib::loaded_settings::loaded_settings(
        const fostlib::string &name, const fostlib::string &banner)
: name(name),
  banner(banner),
  c_ini_file(L"fost-cli/main.cpp", name, L"IniFile", name + L".ini", true),
  c_json_file(L"fost-cli/main.cpp", name, L"JSON File", name + L".json", true),
  c_banner(L"fost-cli/main.cpp", name, L"Banner", true, true),
  c_settings(L"fost-cli/main.cpp", name, L"Settings", false, true),
  c_environment(L"fost-cli/main.cpp", name, L"Environment", false, true),
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
    args.commandSwitch(L"b", settings.name, L"Banner");
    if (settings.c_banner.value()) out << settings.banner << std::endl;

    args.commandSwitch(L"s", settings.name, L"Settings");
    if (settings.c_settings.value()) setting<json>::printAllOn(out);

    args.commandSwitch(L"e", settings.name, L"Environment");
    if (settings.c_environment.value()) args.printOn(out);
}


namespace {
    int exception_wrapper(fostlib::ostream &out, std::function<int(void)> f) {
        try {
            fostlib::exceptions::structured_handler handler;
#ifdef WIN32
            fostlib::ini_file base_settings_file(
                    fostlib::arguments::environment("windir") + L"\\fost.ini");
#else
            fostlib::ini_file base_settings_file(L"/etc/fost.conf");
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
        args.commandSwitch(L"i", settings.c_ini_file);
        fostlib::ini_file ifile(settings.c_ini_file.value());
        args.commandSwitch(L"j", settings.name, "JSON File");
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
