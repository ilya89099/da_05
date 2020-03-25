#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include "SuffixArray.h"

#define GUARD_SYMBOL '$'

class SuffixTree {
public:

    explicit SuffixTree(std::string);
    std::set<long long> SearchForString(const std::string& sought) const;
    SuffixArray ConstructArray() const;
    ~SuffixTree();

private:

    struct SuffixNode;
    struct ArcInfo {
        ArcInfo() = delete;
        ArcInfo(const std::string* str, SuffixNode* link, long long arc_start, long long arc_length);

        bool OnArc();
        bool CheckNextSymbol(char s);
        ArcInfo GoDown(std::pair<long long, long long>);
        std::pair<long long, long long> AddNode(long long index, SuffixNode*& prev);

        SuffixNode* link_ = nullptr;
        long long arc_start_;
        long long arc_length_;

        const std::string* str_;
    };


    void LexicographicalTraverse(SuffixNode* node, std::vector<long long>& result) const;

    void FindOcurencies(SuffixNode* node, std::set<long long>& set) const;

    struct SuffixNode {
        SuffixNode() = default;
        SuffixNode(SuffixNode* parent);

        std::map<char, ArcInfo> links_;
        SuffixNode* parent_ = nullptr;
        SuffixNode* suffix_link_ = nullptr;
    };


    void DeleteSubtree(SuffixNode* node);
    void NumerateLeaves(SuffixNode* node, long long depth);

    std::unordered_map<SuffixNode*, long long> numeration;
    std::string data;
    SuffixNode* giglet;
    SuffixNode* root;



};