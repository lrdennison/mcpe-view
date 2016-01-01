#ifndef _SINGLETON_H
#define _SINGLETON_H

template<typename T>
class Singleton
{
public:
  static T &Inst()
  {
    static T* m_instance=0;

    if(!m_instance) {
      m_instance = new T();
    }

    return *m_instance;
  }

};


#endif
