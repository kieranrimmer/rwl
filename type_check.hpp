//
// Created by Kieran Rimmer on 27/5/17.
//

#ifndef RWL_TYPE_CHECK_HPP
#define RWL_TYPE_CHECK_HPP

#include "AST/tree.hpp"
#include "string_table/string_table.hpp"


namespace RWL {

    enum RwlTypes {INT =0, STRING=1, BOOLEAN, VOID};

    class TypeCheck {
    protected:


    public:
        static RwlTypes checkType(Symbol type) {
            std::string typeString = type->get_string();
            std::cout << "RwlTypes::checkType() -- typeString = " << typeString << std::endl;
            if (typeString.compare("string")) return RwlTypes::STRING;
            else if (typeString.compare("int")) return RwlTypes::INT;
            else if (typeString.compare("boolean")) return RwlTypes::BOOLEAN;
        }


    };

}


#endif //RWL_TYPE_CHECK_HPP



