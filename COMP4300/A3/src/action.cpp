#include "action.h"

Action::Action() = default;

Action::Action(Action::Name name, Action::Type type) : m_name(name), m_type(type)
{
}

Action::Name Action::name() const
{
    return m_name;
}

Action::Type Action::type() const
{
    return m_type;
}
