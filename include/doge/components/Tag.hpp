#pragma once

#include <set>
#include <string>

namespace doge
{
    class Tag
    {
        std::set<std::string> tags;

        bool is_modified = true;

    public:

        Tag(const Tag&) = default;
        Tag(const std::set<std::string>& tags = std::set<std::string>());

        std::set<std::string>& GetTags();
        const std::set<std::string>& GetTags() const;

        std::set<std::string>& _NoModify_GetTags();
        void _ResetModify();
        bool _IsModified() const;
    };
}