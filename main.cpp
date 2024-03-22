#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <QArrayData>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>

enum eClass : short { Warrior, Mage, Archer };

class Player
{
    Q_GADGET
private:
    std::string m_name = "Player";
    int m_level = 1;
    int m_hp = 10;
    int m_mana = 10;
    int m_stamina = 10;
    eClass m_class = eClass::Warrior;
public:
    Player();
    Player(const std::string& name, int level, int hp, int mana, int stamina, eClass playerClass);
    ~Player();
public:
    friend std::ostream &operator<<(std::ostream& stream, Player player);
public:
    static Player fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};



// DEF



Player::Player(const std::string& name,
               int level,
               int hp,
               int mana,
               int stamina,
               eClass playerClass) :
    m_name { name },
    m_level { level },
    m_hp { hp },
    m_mana { mana },
    m_stamina { stamina },
    m_class { playerClass }
{ }
Player::Player() : Player(std::string(""), 1, 10, 10, 10, eClass::Warrior) { }
Player::~Player() { }

std::ostream &operator<<(std::ostream& stream, Player player)
{
    std::string pClass = player.m_class == 0 ? "Воин" : player.m_class == 1 ? "Маг" : "Лучник";
    stream << "Имя: " << player.m_name.c_str()
           << "\nКласс: " << pClass.c_str()
           << "\nУровень: " << player.m_level
           << "HP/MANA/STAMINA: " << player.m_hp << "/" << player.m_mana << "/" << player.m_stamina
           << std::endl;
    return stream;
}

QJsonObject Player::toJson() const
{
    QJsonObject json;
    json["name"] = m_name.c_str();
    json["level"] = m_level;
    json["class"] = m_class;
    json["hp"] = m_hp;
    json["mana"] = m_mana;
    json["stamina"] = m_stamina;
    return json;
}

Player Player::fromJson(const QJsonObject &json)
{
    Player result;

    if (const QJsonValue v = json["name"]; v.isString())
        result.m_name = v.toString().toStdString();

    if (const QJsonValue v = json["level"]; v.isDouble())
        result.m_level = v.toInt();

    if (const QJsonValue v = json["class"]; v.isDouble())
        result.m_class = eClass(v.toInt());
    
    if (const QJsonValue v = json["hp"]; v.isDouble())
        result.m_hp = v.toInt();

    if (const QJsonValue v = json["mana"]; v.isDouble())
        result.m_mana = v.toInt();

    if (const QJsonValue v = json["stamina"]; v.isDouble())
        result.m_stamina = v.toInt();

    return result;
}



// MAIN



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile save("player.json");

    if (!save.exists())
    {
        std::string name = "";
        int choice = 0;
        eClass pClass = eClass::Warrior;

        std::cout << "Введите имя персонажа: " << std::endl;
        std::cin >> name;

        std::cout << "Выберите класс:\n"
                  << "1. Воин\n"
                  << "2. Маг\n"
                  << "3. Лучник\n"
                  << std::endl;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            pClass = eClass::Warrior;
            break;
        case 2:
            pClass = eClass::Mage;
            break;
        case 3:
            pClass = eClass::Archer;
            break;
        default:
            break;
        }

        Player player(name, 1, 10, 10, 10, pClass);
        std::cout << player;

        save.open(QIODevice::WriteOnly);
        save.write(QJsonDocument(player.toJson()).toJson());
        save.close();
    }
    else
    {
        save.open(QIODevice::ReadOnly);
        QJsonDocument playerData = QJsonDocument::fromJson(save.readAll());
        save.close();
        Player p(Player::fromJson(playerData.object()));
        std::cout << p;
    }
    

    return a.exec();
}
