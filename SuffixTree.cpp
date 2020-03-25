
#include "SuffixTree.h"
#include <array>



SuffixTree::SuffixTree(std::string incoming_string)
: data(std::move(incoming_string)), giglet(new SuffixNode), root(new SuffixNode) {

    using Arc = ArcInfo;
    data.push_back(GUARD_SYMBOL);
    for (size_t i = 0; i < data.size(); ++i) {
        giglet->links_.emplace(std::piecewise_construct, std::forward_as_tuple(data[i]), std::forward_as_tuple(&data, root, i, 1));
    }
    root->suffix_link_ = giglet;

    Arc position = Arc(&data, root, -1, 0);
    SuffixNode* prev_created = nullptr;
    for (size_t i = 0; i < data.size(); ++i) {
        char symbol = data[i];
        while (!position.CheckNextSymbol(symbol)) {
            std::pair<long long, long long> arc = position.AddNode(i, prev_created);
            position = Arc(&data, position.link_->suffix_link_, 0, 0).GoDown(arc);
            if (!position.OnArc() && prev_created != nullptr && prev_created->suffix_link_ == nullptr) {
                prev_created->suffix_link_ = position.link_;
            }
        }
        position = position.GoDown({i, 1});
    }
    NumerateLeaves(root, 0);
}

SuffixTree::~SuffixTree() {
    DeleteSubtree(root);
    delete giglet;
}

SuffixArray SuffixTree::ConstructArray() const {
    std::vector<long long> rawSuffixArray;
    LexicographicalTraverse(root, rawSuffixArray);
    return SuffixArray(rawSuffixArray, data.substr(0,data.size() - 1));
}

void SuffixTree::LexicographicalTraverse(SuffixNode* node, std::vector<long long>& result) const {
    if (node->links_.empty()) {
        if (numeration.at(node) != data.size()) {
            result.push_back(numeration.at(node));
        }
    } else {
        for (std::pair<const char, ArcInfo>& element : node->links_) {
            LexicographicalTraverse(element.second.link_, result);
        }
    }
}

std::set<long long> SuffixTree::SearchForString(const std::string& sought) const {
    if (sought.size() > data.size()) {
        return {};
    }
    std::set<long long> result;
    SuffixNode* cur_node = root;
    size_t cur_index = 0;
    while (cur_index < sought.size()) {
        char symbol = sought[cur_index];
        if (cur_node->links_.count(sought[cur_index]) == 0) {
            return {};
        }
        for (size_t i = 0; i < std::min<size_t>(sought.size() - cur_index, cur_node->links_.at(symbol).arc_length_); ++i) {
            if (sought[cur_index + i] != data[cur_node->links_.at(symbol).arc_start_ + i]) {
                return {};
            }
        }
        cur_index += std::min<size_t>(sought.size() - cur_index, cur_node->links_.at(symbol).arc_length_);
        cur_node = cur_node->links_.at(symbol).link_;
    }
    FindOcurencies(cur_node, result);
    return result;
}

void SuffixTree::FindOcurencies(SuffixNode* node, std::set<long long>& set) const {
    if (node->links_.empty()) {
        set.insert(numeration.at(node));
        return;
    }
    for (auto i : node->links_) {
        FindOcurencies(i.second.link_, set);
    }
}


void SuffixTree::DeleteSubtree(SuffixNode* node) {
    for (auto i : node->links_) {
        DeleteSubtree(i.second.link_);
    }
    delete node;
}

void SuffixTree::NumerateLeaves(SuffixNode* node, long long depth){
    if (node->links_.empty()) {
        numeration[node] = data.size() - depth + 1;
        return;
    }
    for (auto i : node->links_) {
        NumerateLeaves(i.second.link_, depth + i.second.arc_length_);
    }
}

SuffixTree::SuffixNode::SuffixNode(SuffixTree::SuffixNode *parent)
: parent_(parent) {}

SuffixTree::ArcInfo::ArcInfo(const std::string* str, SuffixNode* link, long long arc_start, long long arc_length)
: link_(link), arc_start_(arc_start), arc_length_(arc_length), str_(str) {
    if (link == nullptr) {
        throw std::logic_error("shit");
    }
}

bool SuffixTree::ArcInfo::OnArc() {
    return arc_length_ != 0;
}
bool SuffixTree::ArcInfo::CheckNextSymbol(char s) {
    if (OnArc()) {
        return (*str_)[arc_start_ + arc_length_] == s;
    } else {
        return link_->links_.count(s) != 0;
    }
}

SuffixTree::ArcInfo SuffixTree::ArcInfo::GoDown(std::pair<long long, long long> arc) {
    if (arc.second == 0) {
        return *this;
    }

    if (arc_length_ != 0) {
        arc = {arc.first - arc_length_, arc_length_ + arc.second};
    }

    SuffixNode* node = link_;
    while (true) {
        std::pair<long long, long long> node_arc = {node->links_.at((*str_)[arc.first]).arc_start_, node->links_.at((*str_)[arc.first]).arc_length_};
        if (node_arc.second < arc.second) {
            node = node->links_.at((*str_)[arc.first]).link_;
            arc = {arc.first + node_arc.second, arc.second - node_arc.second};
        } else if (node_arc.second > arc.second) {
            return ArcInfo(str_, node, node->links_.at((*str_)[arc.first]).arc_start_, arc.second);
        } else {
            return ArcInfo(str_, node->links_.at((*str_)[arc.first]).link_, 0, 0);
        }

    }

}



std::pair<long long, long long> SuffixTree::ArcInfo::AddNode(long long index, SuffixNode*& prev) {
    if (arc_length_ == 0) {
        if (link_->links_.count((*str_)[index]) == 1) {
            throw std::logic_error("replacing of an existing leaf");
        } else {
            link_->links_.emplace(std::piecewise_construct,
                                  std::forward_as_tuple((*str_)[index]),
                                  std::forward_as_tuple(str_, new SuffixNode(link_), index, (long long)str_->size() - index));
        }
        return {0,0};
    } else {
        std::pair<long long, long long> first_arc = {arc_start_, arc_length_};
        std::pair<long long, long long> second_arc = {arc_start_ + arc_length_, link_->links_.at((*str_)[arc_start_]).arc_length_ - arc_length_};
        SuffixNode* new_node = new SuffixNode(link_);
        SuffixNode* child = link_->links_.at((*str_)[arc_start_]).link_;
        child->parent_ = new_node;
        link_->links_.at((*str_)[arc_start_]) = {str_, new_node, first_arc.first, first_arc.second};
        new_node->links_.emplace(std::piecewise_construct, std::forward_as_tuple((*str_)[second_arc.first]), std::forward_as_tuple(str_, child, second_arc.first, second_arc.second));
        new_node->links_.emplace(std::piecewise_construct, std::forward_as_tuple((*str_)[index]), std::forward_as_tuple(str_, new SuffixNode(new_node), index, (long long)str_->size() - index));
        if (prev != nullptr && prev->suffix_link_ == nullptr) {
            prev->suffix_link_ = new_node;
        }
        prev = new_node;
        return first_arc;
    }
}