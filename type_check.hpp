//
// Created by Kieran Rimmer on 27/5/17.
//

#ifndef RWL_TYPE_CHECK_HPP
#define RWL_TYPE_CHECK_HPP

#include "AST/tree.hpp"
#include "string_table/string_table.hpp"


namespace RWL {

    enum RwlTypes {INT =0, STRING=1, BOOLEAN, VOID, NO_TYPE};

    class TypeCheck {
    protected:


    public:
        static RwlTypes checkType(Symbol type) {
            if (!type)
                return  RwlTypes::NO_TYPE;
            std::string typeString = type->get_string();
            std::cout << "RwlTypes::checkType() -- typeString = " << typeString << std::endl;
//            int c1 = typeString.compare("string");
            if (typeString.compare("string") == 0) return RwlTypes::STRING;
            else if (typeString.compare("int") == 0) return RwlTypes::INT;
            else if (typeString.compare("boolean") == 0) return RwlTypes::BOOLEAN;
            else return RwlTypes::NO_TYPE;
        }


    };

}


#endif //RWL_TYPE_CHECK_HPP



