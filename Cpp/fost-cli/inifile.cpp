/**
    Copyright 1998-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-cli.hpp"
#include <fost/inifile.hpp>
#include <fost/insert>
#include <fost/unicode>


using namespace fostlib;


/**
    ## fostlib::exceptions::invalid_ini_line
*/


fostlib::exceptions::invalid_ini_line::invalid_ini_line(
        const fostlib::string &section,
        const fostlib::string &read,
        const fostlib::string &processed) noexcept
: exception() {
    fostlib::insert(data(), "section", section);
    fostlib::insert(data(), "ini-file", read);
    fostlib::insert(data(), "processed", processed);
}


const wchar_t *const
        fostlib::exceptions::invalid_ini_line::message() const noexcept {
    return L"Invalid line in INI file";
}


/**
    ## fostlib::ini_file
*/


fostlib::ini_file::ini_file(string const &a_fileName) : m_fileName{a_fileName} {
    loadAll();
}


void fostlib::ini_file::loadAll() {
    string section, line,
            file(utf::load_file(
                    coerce<fs::path>(m_fileName), fostlib::string{}));

    for (utf32 const c : file) {
        if (c == '\n' || c == '\r') {
            fostlib::string read(line);
            line = line.substr(0, line.find_last_not_of(" \t") + 1);
            if (line.find_first_not_of(" \t") != string::npos)
                line = line.substr(line.find_first_not_of(" \t"));
            if (line == "") {
                // Blank line - ignore it
            } else if (line[0] == ';') {
                // Comment line - ignore it
            } else if (line[0] == '[') {
                section = line.substr(1, line.code_points() - 2);
                if (section == "" || line.find(']') == string::npos
                    || section.find_first_of("[]") != string::npos)
                    throw exceptions::invalid_ini_line("", read, section);
            } else {
                fostlib::string::size_type pos(line.find('='));
                if (pos == string::npos || section == "")
                    throw exceptions::invalid_ini_line(section, read, line);
                fostlib::string name(line.substr(0, pos)),
                        value(line.substr(pos + 1));
                json js = json::parse(value, json());
                if (!value.empty() && js.isnull()) {
                    m_settings.push_back(std::make_shared<setting<string>>(
                            m_fileName, section, name, value, false));
                } else {
                    m_settings.push_back(std::make_shared<setting<json>>(
                            m_fileName, section, name, js, false));
                }
            }
            line = "";
        } else
            line += c;
    }
}
