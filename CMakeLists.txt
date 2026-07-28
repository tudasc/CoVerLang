cmake_minimum_required(VERSION 3.28)

project(CoVerLang LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get ANTLR
find_package(antlr4-generator QUIET)
find_package(antlr4-runtime QUIET)

if (NOT antlr4-generator_FOUND)
  set(ANTLR4_BAKED_VERSION 4.13.2)
  set(ANTLR4_JAR_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Externals/antlr-${ANTLR4_BAKED_VERSION}-complete.jar" CACHE STRING "Location of antlr4 jar file")
  set(ANTLR_BUILD_CPP_TESTS OFF)
  set(ANTLR_BUILD_SHARED OFF)
  include(FetchContent)
  set(ANTLR4_INSTALL ON)
  FetchContent_Declare(
    antlr4
    GIT_REPOSITORY https://github.com/antlr/antlr4
    GIT_TAG        "${ANTLR4_BAKED_VERSION}"
    SOURCE_SUBDIR runtime/Cpp
    QUIET
  )
  FetchContent_MakeAvailable(antlr4)
  target_compile_options(antlr4_static PUBLIC -fPIC)
  set(antlr4-generator_DIR "${antlr4_BINARY_DIR}")
  find_package(antlr4-generator REQUIRED)
  set(ANTLR4_INCLUDE_DIR "${antlr4_SOURCE_DIR}/runtime/Cpp/runtime/src")
  set(ANTLR4_LIBRARY antlr4_static CACHE STRING "ANTLR4 target to use")
else()
  set(ANTLR4_JAR_LOCATION "/usr/share/java/antlr-complete.jar" CACHE STRING "Location of antlr4 jar file")
  set(ANTLR4_LIBRARY antlr4-runtime CACHE STRING "ANTLR4 target to use")
endif(NOT antlr4-generator_FOUND)

# Generate Parser
antlr4_generate(ContractLex ${CMAKE_CURRENT_SOURCE_DIR}/Grammars/ContractLexer.g4 LEXER)
antlr4_generate(ContractPars ${CMAKE_CURRENT_SOURCE_DIR}/Grammars/ContractParser.g4 PARSER "" "VisitorArg" "" "${ANTLR4_TOKEN_FILES_ContractLex}" "${ANTLR4_TOKEN_DIRECTORY_ContractLex}")

# Library
add_library(CoVerLang
  ${ANTLR4_SRC_FILES_ContractLex}
  ${ANTLR4_SRC_FILES_ContractPars}
)
target_sources(CoVerLang PRIVATE FILE_SET coverlang_cxx_modules TYPE CXX_MODULES FILES
  LangCode/ContractDataVisitor.cppm
  LangCode/CoVerLangErrorListener.cppm
  Include/ContractDataExtractor.cppm
)
target_include_directories(CoVerLang PRIVATE
    Include/
    ${ANTLR4_INCLUDE_DIR_ContractLex}
    ${ANTLR4_INCLUDE_DIR_ContractPars}
    ${ANTLR4_INCLUDE_DIR}
)

set_property(TARGET CoVerLang PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS CoVerLang)
