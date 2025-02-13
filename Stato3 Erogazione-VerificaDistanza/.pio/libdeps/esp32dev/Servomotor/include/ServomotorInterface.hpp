#ifndef SERVOMOTOR_INTERFACE_HPP
#define SERVOMOTOR_INTERFACE_HPP

class ServomotorInterface
{
    public:
        virtual void set_position_up() = 0;
        virtual void set_position_down() = 0;
        virtual void set_angle(const float angle_degree) = 0;
};

#endif