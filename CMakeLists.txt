cmake_minimum_required(VERSION 3.27)
project(HomeAccountingDB)

set(CMAKE_CXX_STANDARD 23)

add_executable(HomeAccountingDB main.cpp
        json/json_parser.cpp
        json/json_parser.h
        json/json_object_parser.cpp
        json/json_object_parser.h
        entities/accounts.cpp
        entities/accounts.h
        json/json_object_array_parser.h
        entities/subcategories.cpp
        entities/subcategories.h
        entities/common.h
        entities/finance_operations.cpp
        entities/finance_operations.h
        entities/common.cpp
        core/time_series_data.h
        db.cpp
        db.h
        core/object_array.h
        jsonconfig/accounts.cpp
        jsonconfig/accounts.h
        jsonconfig/subcategories.cpp
        jsonconfig/subcategories.h
        jsonconfig/finance_operations.cpp
        jsonconfig/finance_operations.h
        jsonconfig/json_db_config.cpp
        jsonconfig/json_db_config.h
        db_config.h
        core/lru.h)
