#include <doge/components/Tag.hpp>

namespace doge
{
    Tag::Tag(const std::set<std::string>& tags)
        : tags(tags)
    {
    }

    std::set<std::string>& Tag::GetTags()
    {
        is_modified = true;
        return tags;
    }

    const std::set<std::string>& Tag::GetTags() const
    {
        return tags;
    }

    std::set<std::string>& Tag::_NoModify_GetTags()
    {
        return tags;
    }

    void Tag::_ResetModify()
    {
        is_modified = false;
    }

    bool Tag::_IsModified() const
    {
        return is_modified;
    }
}