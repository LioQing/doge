#include <doge/components/Sprite.hpp>

#include <doge/core/Assets.hpp>
#include <doge/core/io/Texture.hpp>
#include <doge/utils/math.hpp>

namespace doge
{
    Recti Sprite::GetActualRectangle(const Assets& assets) const
    {
        auto& texture = assets.GetTexture(texture_id);
        Recti rect = texture_rectangle;
        if (!atlas_rectangle_id.empty())
        {
            rect = texture.atlas_rectangles.at(atlas_rectangle_id);
        }
        return math::AutoSize(rect, texture.GetSize());
    }

    Vec2f Sprite::GetActualSize(const Assets& assets) const
    {
        return math::AutoSize(size, GetActualRectangle(assets).GetSize());
    }
}