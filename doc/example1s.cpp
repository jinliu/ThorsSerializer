#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/SerUtil.h"

namespace X
{
struct Color
{
    int     red;
    int     green;
    int     blue;
};    
enum Gender
{
    Male,
    Female,
    Misc
};
class TeamMember
{
    std::string     name;
    int             score;
    int             damage;
    Color           team;
    Gender     gender;
    std::vector<float> scores;
    public:
        TeamMember(std::string const& name, int score, int damage, Color const& team, Gender gender)
            : name(name)
            , score(score)
            , damage(damage)
            , team(team)
            , gender(gender)
        {}
        // Define the trait as a friend to get accesses to private
        // Members.
        friend class ThorsAnvil::Serialize::Traits<TeamMember>;
};
class ExTeamMember : public TeamMember
{
    int retireYear;
    public:
        ExTeamMember()
            : TeamMember("unknown", 0, 0, X::Color{255, 0, 0}, X::Female)
        {}
        ExTeamMember(std::string const& name, int score, int damage, Color const& team, Gender gender, int retireYear)
            : TeamMember(name, score, damage, team, gender),
            retireYear(retireYear)
        {}
        // Define the trait as a friend to get accesses to private
        // Members.
        friend class ThorsAnvil::Serialize::Traits<ExTeamMember>;
};
}

// Declare the traits.
// Specifying what members need to be serialized.
ThorsAnvil_MakeTrait(X::Color, red, green, blue);
ThorsAnvil_MakeEnum(X::Gender, Male, Female);
ThorsAnvil_MakeTrait(X::TeamMember, name, score, damage, team, gender,scores);
ThorsAnvil_ExpandTrait(X::TeamMember, X::ExTeamMember, retireYear);
int main()
{
    using ThorsAnvil::Serialize::jsonSchemaExtract;

    X::TeamMember          mark("mark", 10, 5, X::Color{255,0,0}, X::Male);
    std::cout << jsonSchemaExtract(mark) << "\n";
    std::cout << jsonSchemaExtract<X::ExTeamMember>() << "\n";

}

