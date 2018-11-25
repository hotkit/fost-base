/*
    Copyright 1998-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INIFILE_HPP
#define FOST_INIFILE_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/settings.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CLI_DECLSPEC invalid_ini_line : public exception {
          public:
            invalid_ini_line(
                    const string &section,
                    const string &read,
                    const string &processed) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


    class FOST_CLI_DECLSPEC ini_file {
      public:
        ini_file(const string &a_fileName);

      private:
        void loadAll();

        fostlib::string m_fileName;
        std::list<boost::shared_ptr<setting<json>>> m_settings;
    };


}


#endif // FOST_INIFILE_HPP
