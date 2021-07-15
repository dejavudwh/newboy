#ifndef __NEWBOY_CONFIG_H__
#define __NEWBOY_CONFIG_H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
#include "log.h"

namespace newboy {

class ConfigVarBase {
    public:
        typedef std::shared_ptr<ConfigVarBase> ptr;

        ConfigVarBase(const std::string& name, const std::string& description = "")
            :m_name(name),
             m_description(description) {}
        virtual ~ConfigVarBase() {}

        const std::string& getName() const { return m_name; }
        const std::string& getDescription() const { return m_description; }

        virtual std::string toString() = 0;
        virtual bool fromString(const std::string& val) = 0;
     
    protected:
        std::string m_name;
        std::string m_description;
};

template<class T>
class ConfigVar : public ConfigVarBase {
    public:
        typedef std::shared_ptr<ConfigVar> ptr;

        ConfigVar(const std::string& name,
                  const T& default_value,
                  const std::string& description)
            :ConfigVarBase(name, description),
             m_val(default_value) {}

        std::string toString() override {
            try {
                return boost::lexical_cast<std::string>(m_val);
            } catch(std::exception& e) {
                NEWBOY_LOG_ERROR(NEWBOY_LOG_ROOT()) << "ConfigVar::toString exception"
                    << e.what() << "convert: string to " << typeid(m_val).name();
            }

            return "";
        }
        bool fromString(const std::string& val) override {
            try {
                m_val = boost::lexical_cast<T>(val);
            } catch(std::exception& e) {
                NEWBOY_LOG_ERROR(NEWBOY_LOG_ROOT()) << "ConfigVar::toString exception"
                    << e.what() << "convert: string to " << typeid(m_val).name();
            }

            return false;
        }

        const T getValue() const { return m_val; }
        void setValue(const T& v) { m_val = v; }
    private:
        T m_val;
};

class Config {
    public:
        typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string& name,
                                                 const T& default_value,
                                                 const std::string& description) {
            auto tmp = Lookup<T>(name);
            if (tmp) {
                NEWBOY_LOG_INFO(NEWBOY_LOG_ROOT()) << "Lookup name= " << name << " exists";
                
                return tmp;
            }

            if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._0123456789")
                != std::string::npos) {
                NEWBOY_LOG_ERROR(NEWBOY_LOG_ROOT()) << "Lookup name invalid " << name;
                throw std::invalid_argument(name);
            }           

            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
            m_datas[name] = v;       

            return v;                           
        }
        
        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
            auto it = m_datas.find(name);
            if (it == m_datas.end()) {
                return nullptr;
            }                  

            return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);                          
        }
    private:
        static ConfigVarMap m_datas;
};

}

#endif