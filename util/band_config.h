/* Copyright 2025 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */


#ifndef BAND_CONFIG_H_
#define BAND_CONFIG_H_

#include "api.h"

#ifndef DEFAULT_DECT_BAND_GROUP 
#define DEFAULT_DECT_BAND_GROUP (1)
#endif


/**
 * \brief   Configure DECT Band Group (ETSI TS 103 636-2)
 * \param   band group index (supported 1,4 or 9)
 * \returns @ref APP_RES_OK if the configuration is successful, an error
 *          code otherwise */
__STATIC_INLINE app_res_e BandConfig_setDectBandGroup(uint8_t band_group)
{
#ifdef DECT_BAND_CONFIG    
   uint32_t band_mask;
   
   if (band_group == 1)
   {
       band_mask = APP_LIB_DECT_BAND_1;
   }
   else if (band_group == 4)
   {
       band_mask = APP_LIB_DECT_BAND_4;
   }
   else 
   {
       return APP_RES_INVALID_VALUE;
   }
   return lib_radio_cfg->bandSetup(band_mask);
#else
   (void)band_group;
   return APP_RES_NOT_IMPLEMENTED;
#endif
}

#endif

