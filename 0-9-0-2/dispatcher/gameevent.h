#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include <string>
#include <vector>
#include <typeinfo>

class EventBase {
protected:
    std::string name_;
    std::vector<std::string> types_;
    bool has_data;

public:
    std::string              getName()  const { return name_; }
    std::vector<std::string> getTypes() const { return types_; }
    virtual std::string      getDataTypeName() const = 0;
    bool operator==(EventBase & that) {
        return (this->name_.compare(that.getName()) == 0);
    }

};

template <class T> class GameEvent : public EventBase
{
    T data_;
    std::string data_type_name;

private:
    // void initDataTypeName() { data_type_name = typeid(T).name(); }

public:
    GameEvent():
        data_type_name(typeid(T).name()) {}
    GameEvent(std::string name,
              std::vector<std::string> types,
              T data):
          data_(data),
          data_type_name(typeid(T).name()) {
        name_ = name;
        types_ = types;
        has_data= true;
    }

    T                        getData()  const { return data_; }
    std::string              getDataTypeName() const { return data_type_name; }
    void setData(T data) { data_ = data; has_data = true; }
    T    getData() { return data_; }
    void clearData() { data_ = T(); has_data = false; }

    //typedef typeof(T) data_type;
    using data_type = T;
    bool hasData() { return has_data; }
};

#endif // GAMEEVENT_H
