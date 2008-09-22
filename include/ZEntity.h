/*
 * ZEntity.h
 *
 *  Created on: Sep 18, 2008
 *      Author: bey0nd
 */

#ifndef ZENTITY_H_
#define ZENTITY_H_

#include <string>

using namespace std;

namespace ZGame
{
  class ZEntity
  {
  public:
    ZEntity(const string entName,const string meshName);
    ~ZEntity();

    const string getEntityName() {return _entName;}
    const string getMeshName() {return _meshName;}

  protected:

    string _entName;
    string _meshName;

  private:

  };
}

#endif /* ZENTITY_H_ */
