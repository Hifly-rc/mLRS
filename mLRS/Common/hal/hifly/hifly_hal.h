
#ifndef HIFLY_HAL_H
#define HIFLY_HAL_H

//-- MLRS Hifly hal
//-- RX
#ifdef RX_HIFLY_WLE5CC
#if defined DEVICE_NAME_SE77RX10 || defined DEVICE_NAME_SE77RX11
#include "rx-hal-hifly-e77rx1.0-wle5cc.h"
#endif
#ifdef DEVICE_NAME_GZSE77RX10
#include "rx-hal-gz-e77rx1.0-wle5cc.h"
#endif
#endif

#ifdef RX_HIFLY_F103CB
#ifdef DEVICE_NAME_SE19RX10
#include "rx-hal-hifly-e19rx1.0-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE28RX12
#include "rx-hal-hifly-e28rx1.2-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE28RX20
#include "rx-hal-hifly-e28rx2.0-f103cb.h"
#endif
#if defined DEVICE_NAME_SE22RX12 || defined DEVICE_NAME_SE220RX12
#include "rx-hal-hifly-e22rx1.2-f103cb.h"
#endif
#if defined DEVICE_NAME_SE28RX21 || defined DEVICE_NAME_DE28RX21
#include "rx-hal-hifly-e28rx2.1-f103cb.h"
#endif
#if defined DEVICE_NAME_SE22RX21 ||defined DEVICE_NAME_SE220RX21
#include "rx-hal-hifly-e22rx2.1-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE32RX21
#include "rx-hal-hifly-e32rx2.1-f103cb.h"
#endif
#endif

//-- TX
#ifdef TX_HIFLY_WLE5CC
#if defined DEVICE_NAME_SE77TX10 || defined DEVICE_NAME_SE77TX11
#include "tx-hal-hifly-e77tx1.0-wle5cc.h"
#endif
#if defined DEVICE_NAME_GZSE77TX11 || defined  DEVICE_NAME_GZSE77TX10
#include "tx-hal-gz-e77tx1.1-wle5cc.h"
#endif
#endif
#ifdef TX_HIFLY_F103CB
#if defined DEVICE_NAME_SE28TX12 || defined DEVICE_NAME_SE28TX12S
#include "tx-hal-hifly-e28tx1.2-f103cb.h"
#endif
#if defined DEVICE_NAME_SE220TX12 || defined DEVICE_NAME_SE220TX12S
#include "tx-hal-hifly-e22tx1.2-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE28TX20
#include "tx-hal-hifly-e28tx2.0-f103cb.h"
#endif
#if defined DEVICE_NAME_SE22TX20 || defined DEVICE_NAME_SE220TX20
#include "tx-hal-hifly-e22tx2.0-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE28TX21
#include "tx-hal-hifly-e28tx2.1-f103cb.h"
#endif
#ifdef DEVICE_NAME_DE28TX21
#include "tx-hal-hifly-e28tx2.1-f103cb.h"
#endif
#if defined DEVICE_NAME_SE22TX21 || defined DEVICE_NAME_SE220TX21
#include "tx-hal-hifly-e22tx2.1-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE32TX21
#include "tx-hal-hifly-e32tx2.1-f103cb.h"
#endif
#ifdef DEVICE_NAME_SE22TX22
#include "tx-hal-hifly-e22tx2.2-f103cb.h"
#endif
#ifdef DEVICE_NAME_XLITE_SE28TX22
#include "tx-hal-xlite-e28tx2.2-f103cb.h"
#endif
#endif

#endif
