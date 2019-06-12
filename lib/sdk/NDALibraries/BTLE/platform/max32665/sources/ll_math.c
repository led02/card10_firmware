/*******************************************************************************
* Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*
* $Id: ll_math.c 41190 2019-02-25 17:25:06Z zach.metzinger $
*
********************************************************************************
*/

#include "ll_math.h"
#include "max32665.h"
#include "trng_regs.h"
#include "tpu_regs.h"
#include "gcr_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define MXC_F_TPU_CTRL_DONE_FLAGS      (MXC_F_TPU_CTRL_DMA_DONE | MXC_F_TPU_CTRL_GLS_DONE | MXC_F_TPU_CTRL_HSH_DONE | MXC_F_TPU_CTRL_CPH_DONE | MXC_F_TPU_CTRL_MAA_DONE)
#define AES_BLOCK_SIZE  16

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

uint32_t dbb_event_get_current_tstamp(void); /* FIXME: Intial parts didn't have TRNG enabled, had to roll our own */

/*************************************************************************************************/
/*!
 *  \brief  Generate random number.
 *
 *  \return 32-bit random number.
 */
/*************************************************************************************************/
uint32_t LlMathRandNum(void)
{
#if 0

  // Wait for TRNG to be ready
  while(!(MXC_TRNG->st & MXC_F_TRNG_ST_RND_RDY)) {}

  return ((uint32_t)(MXC_TRNG->data));
#else
  static unsigned int rnd = 0;

  /* Sorta-kinda random number generator */
  if (rnd == 0) {
    rnd = dbb_event_get_current_tstamp();
  } else {
    rnd <<= 1;
    rnd ^= dbb_event_get_current_tstamp();
  }

  return rnd;
#endif
}

/******************************************************************************/
void memcpy32(uint32_t * dst, uint32_t * src, unsigned int len)
{
    while (len) {
        *dst = *src;
        dst++;
        src++;
        len -= 4;
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Calculate AES ECB.
 *
 *  \param  pKey        Encryption key.
 *  \param  pOut        Output data.
 *  \param  pIn         Input data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void LlMathAesEcb(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn)
{
  /* Save the state of the crypto clock enable bit */
  uint32_t perckcn0 = MXC_GCR->perckcn0;

  /* Save the state of the HIRC EN */
  uint32_t clkcn = MXC_GCR->clkcn;

  /* Enable CRYPTO clock */
  if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC_EN) == 0) {
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
  }
    
  /* Disable CRYPTO clock gate */
  if (MXC_GCR->perckcn0 & MXC_F_GCR_PERCKCN0_CRYPTOD) {
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_CRYPTOD);
  }

  /* Reset Crypto block and clear state */
  MXC_TPU->ctrl = MXC_F_TPU_CTRL_RST;

  /* Set the legacy bit */
  MXC_TPU->ctrl |= MXC_F_TPU_CTRL_FLAG_MODE;

  /* Byte swap the input and output */
  MXC_TPU->ctrl |= MXC_F_TPU_CTRL_BSO;
  MXC_TPU->ctrl |= MXC_F_TPU_CTRL_BSI;

  MXC_TPU->cipher_ctrl = (MXC_S_TPU_CIPHER_CTRL_MODE_ECB | 
    MXC_S_TPU_CIPHER_CTRL_CIPHER_AES128 | MXC_S_TPU_CIPHER_CTRL_SRC_CIPHERKEY);

  /* Clear all done flags */
  MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DONE_FLAGS;

  /* Load the Key */
  memcpy32((void*)&MXC_TPU->cipher_key[0], (void*)&pKey[12], 4);
  memcpy32((void*)&MXC_TPU->cipher_key[1], (void*)&pKey[8], 4);
  memcpy32((void*)&MXC_TPU->cipher_key[2], (void*)&pKey[4], 4);
  memcpy32((void*)&MXC_TPU->cipher_key[3], (void*)&pKey[0], 4);

  /* Wait until ready for data */
  while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_RDY));

  /* Copy data to start the operation */
  memcpy32((void*)&MXC_TPU->din[0], (void*)&pIn[12], 4);
  memcpy32((void*)&MXC_TPU->din[1], (void*)&pIn[8], 4);
  memcpy32((void*)&MXC_TPU->din[2], (void*)&pIn[4], 4);
  memcpy32((void*)&MXC_TPU->din[3], (void*)&pIn[0], 4);

  /* Wait until operation is complete */
  while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_CPH_DONE));

  /* Clear all done flags */
  MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DONE_FLAGS;
  
  /* Copy data out */
  memcpy32((void*)&pOut[12], (void*)&MXC_TPU->dout[0], 4);
  memcpy32((void*)&pOut[8], (void*)&MXC_TPU->dout[1], 4);
  memcpy32((void*)&pOut[4], (void*)&MXC_TPU->dout[2], 4);
  memcpy32((void*)&pOut[0], (void*)&MXC_TPU->dout[3], 4);

  /* Restore clock settings */
  MXC_GCR->perckcn0 = perckcn0;
  MXC_GCR->clkcn = clkcn;
}
