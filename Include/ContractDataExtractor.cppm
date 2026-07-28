module;

#include "ContractTree.hpp"

export module ContractDataExtractor;

export std::optional<ContractTree::ContractData> getContractData(std::string contract);
