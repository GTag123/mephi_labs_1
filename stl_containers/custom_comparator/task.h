#include <map>
#include <vector>
#include <string>

struct SuperKey {
    std::string StrPart;
    int IntPart;
    float FloatPart;

    bool operator==(const SuperKey& rhs) const {
        return this->StrPart == rhs.StrPart && this->IntPart == rhs.IntPart && this->FloatPart == rhs.FloatPart;
    }
};

bool operator <(const SuperKey& lhs, const SuperKey& rhs);

/*
 * Напишите реализацию оператора < для типа SuperKey, чтобы ключи типа SuperKey можно было использовать в map.
 * Порядок должен быть таким, чтобы все ключи с одинаковым значением поля StrPart шли друг за другом без пропусков.
 * Напишите функцию PopulateMap, которая добавляет в map пары ключ-значение из данного вектора toAdd
 */
void PopulateMap(
    std::map<SuperKey, std::string>& map,
    const std::vector<std::pair<SuperKey, std::string>>& toAdd
);
