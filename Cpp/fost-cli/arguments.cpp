/**
    Copyright 1995-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-cli.hpp"
#include <fost/arguments.hpp>
#include <iostream>


using namespace fostlib;


fostlib::arguments::arguments(int argc, const char *const argv[]) {
    load(argc, argv);
}


fostlib::arguments::arguments(
        int argc, const char *const argv[], const char *const envp[]) {
    load(argc, argv);
    load(envp);
}


void fostlib::arguments::load(int argc, const char *const argv[]) {
    for (int c(0); c < argc; ++c) {
        if (argv[c][0] == '-') {
            if (argc == c + 1)
                m_switches[string(argv[c] + 1)] = "";
            else {
                m_switches[string(argv[c] + 1)] = string(argv[c + 1]);
                ++c;
            }
        } else
            m_arguments.push_back(string(argv[c]));
    }
}


void fostlib::arguments::load(const char *const envp[]) {
    for (int arg(0); envp[arg] != NULL; ++arg) {
        string setting(envp[arg]);
        auto set = partition(setting, "=");
        if (set.second) m_environment[set.first] = set.second.value();
    }
}


string fostlib::arguments::environment(const string &) {
#ifdef FOST_OS_WINDOWS
    DWORD length = GetEnvironmentVariable("windir", NULL, 0);
    if (length) {
        boost::scoped_array<wchar_t> windir(new wchar_t[length]);
        GetEnvironmentVariable("windir", windir.get(), length);
        return string(windir.get());
    } else
        throw exceptions::null("Environment variable WINDIR not found");
#else
    throw exceptions::not_implemented(
            "fostlib::arguments::environment( const string &env_name )");
#endif
}
void fostlib::arguments::environment(
        const string &envName, const string &section, const string &name) {
    if (m_environment.find(envName) != m_environment.end()) {
        m_registered.push_back(
                boost::shared_ptr<setting<json>>(new setting<json>(
                        "Environment", section, name,
                        json(m_environment[envName]), false)));
    }
}


void fostlib::arguments::argument(
        arguments::size_type argument,
        const string &section,
        const string &name) {
    if (argument < size()) {
        m_registered.push_back(
                boost::shared_ptr<setting<json>>(new setting<json>(
                        "Command argument", section, name,
                        json(m_arguments[argument]), false)));
    }
}


nullable<string> fostlib::arguments::commandSwitch(const string &s) const {
    std::map<string, string>::const_iterator pos(m_switches.find(s));
    if (pos == m_switches.end())
        return null;
    else
        return (*pos).second;
}


void fostlib::arguments::commandSwitch(
        const string &theSwitch, const string &section, const string &name) {
    if (m_switches.find(theSwitch) != m_switches.end())
        m_registered.push_back(boost::shared_ptr<setting<json>>(new setting<json>(
                "Command switch", section, name,
                json::parse(m_switches[theSwitch], json(m_switches[theSwitch])),
                false)));
}


void fostlib::arguments::commandSwitch(
        const string &theSwitch, const setting<json> &setting_) {
    if (m_switches.find(theSwitch) != m_switches.end())
        m_registered.push_back(
                boost::shared_ptr<setting<json>>(new setting<json>(
                        "Command switch", setting_,
                        json::parse(
                                m_switches[theSwitch],
                                json(m_switches[theSwitch])))));
}


nullable<string> fostlib::arguments::operator[](arguments::size_type pos) const {
    if (pos >= size()) {
        return nullable<string>();
    } else {
        return m_arguments[pos];
    }
}


ostream &fostlib::arguments::printOn(ostream &o) const {
    for (std::map<string, string>::const_iterator it(m_environment.begin());
         it != m_environment.end(); ++it) {
        o << "argument,environment," << (*it).first << "," << (*it).second
          << std::endl;
    }
    for (std::vector<string>::const_iterator it(m_arguments.begin());
         it != m_arguments.end(); ++it) {
        o << "argument,command line argument," << (*it) << std::endl;
    }
    for (std::map<string, string>::const_iterator it(m_switches.begin());
         it != m_switches.end(); ++it) {
        o << "argument,command line switch," << (*it).first << ","
          << (*it).second << std::endl;
    }
    return o;
}
