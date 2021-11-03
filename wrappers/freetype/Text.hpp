#pragma once

#include <wrappers/gl/Sprite.hpp>
#include <wrappers/gl/Texture.hpp>
#include <wrappers/gl/VertexArray.hpp>
#include <wrappers/freetype/Font.hpp>
#include <snk/lazy_resource.hpp>
#include <glm/vec4.hpp>

/// @brief The renderable part of Font, the String itself.
/// @details Wrap a texture of text.
/// @remarks If not scaled, the unit size of the Text is the pixel.
/// @remarks Supports multiline Strings.
class Text : public Drawable, public Transformable, private snk::lazy_resource
{
public:
    enum Layout
    {
        Zero, ///< Origin is set to zero (default)
        Centered ///< Origin is set to the middle of the text
    };
    
    using Transformable::setOrigin;
    void setOrigin(Layout x, Layout y);
    void setOrigin(Layout xy);
    
    Text();

    /// @brief Set the font. If the font is null, a draw() call will crash.
    void setFont(const Font *font);
    const Font* getFont() const;

    /// @brief Set the string.
    /// @remarks Mulitline strings are supported. For each character '\n' found, a new line is added.
    void setString(const std::string& string);
    const std::string& getString() const;

    void setColor(const glm::vec4& color);
    const glm::vec4& getColor() const;

    void setOutlineColor(const glm::vec4 &color);
    const glm::vec4& getOutlineColor() const;

    void setCharacterSize(int characterSize);
    int getCharacterSize() const;

    float getLineHeight() const;

    /// @brief Set the opacity.
    /// @param opacity 0 is full transparent, 1 is opaque
    /// @details Set the outline and fill color alpha channels.
    void setOpacity(float opacity);

    void setOutline(bool outline);
    bool hasOutline() const;

    void draw(RenderStates states) const override;
    void drawBoundingBox(RenderStates states) const;

    /// @brief Get the size of full text, in pixel.
    /// @remarks The height is (line height) * (the count of lines).
    glm::vec2 getSize() const;

private:
    /// @brief Update the glyph and the sprite based on the font, string and color.
    /// @details Also update the size based of the count of lines and the longest line.
    void update() const override;

    void blitChar(VertexArray& varr, const Glyph& glyph, const glm::vec2& cursor, const glm::vec4& color) const;

    /// @brief Add a rect as 2 triangles in trigonometric order.
    static void addRect(VertexArray& varr, const Rect& pos, const Rect& uvs, const glm::vec4& color);

    /// @name User parameters
    /// @{
    int m_characterSize {32};
    bool m_hasOutline {false};
    const Font *m_font {nullptr};
    std::string m_string;
    glm::vec4 m_color {1.0f};
    glm::vec4 m_outlineColor {1.0f};
    /// @}

    /// @name Internal mutable fields
    /// @brief Elements that are only updated when needed, that's why they are mutable.
    /// @{
    mutable glm::vec2 m_size {0.0f};
    mutable std::vector<VertexArray> m_segments; ///< Portions of the Text, in Text coordinates
    /// @}
};