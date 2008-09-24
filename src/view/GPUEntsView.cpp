/*
 * GPUEntsView.cpp
 *
 *  Created on: Sep 22, 2008
 *      Author: bey0nd
 */

#include "GPUEntsView.h"
#include "GPUEntities.h"
using namespace ZGame;

GPUEntsView::GPUEntsView()
{

}

GPUEntsView::~GPUEntsView()
{

}

/**
 * Initialize this view.
 */
void GPUEntsView::init()
{
  //generate the GPUEntsMesh
}

void GPUEntsView::attachGPUEnts(GPUEntities* ent)
{
  _ents = ent;
}



