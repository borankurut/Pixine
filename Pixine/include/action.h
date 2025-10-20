#pragma once

class Action
{
   public:
    enum class Type
    {
        NONE,
        START,
        END
    };

    enum class Name
    {
        NONE,
        PAUSE,
        QUIT,
        TOGGLE_TEXTURE,
        TOGGLE_COLLISION,
        TOGGLE_GRID,
        JUMP,
        LEFT,
        DOWN,
        RIGHT,
        SHOOT,
        UP,
        PLAY,
    };

    Action();
    Action(Name name, Type type);
    [[nodiscard]] Name name() const;
    [[nodiscard]] Type type() const;

   private:
    Name m_name;
    Type m_type;
};
