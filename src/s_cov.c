/**************************************************************************
*
* Copyright (C) 2008 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "config.h"
#include "bacdef.h"
#include "bacdcode.h"
#include "address.h"
#include "tsm.h"
#include "dcc.h"
#include "npdu.h"
#include "apdu.h"
#include "device.h"
#include "datalink.h"
#include "cov.h"
/* some demo stuff needed */
#include "handlers.h"
#include "txbuf.h"
#include "newclient.h"

/** @file s_cov.c  Send a Change of Value (COV) update or a Subscribe COV request. */

/** Sends a COV Subscription request.
 * @ingroup DSCOV
 * TODO
 * @param device_id [in] ID of the destination device
 * @param cov_data [in]  The COV subscription information to be encoded.
 * @return invoke id of outgoing message, or 0 if communication is disabled or
 *         no slot is available from the tsm for sending.
 */
uint8_t Send_COV_Subscribe_Address(
    BACNET_ADDRESS * dest,
    uint16_t max_apdu,
    BACNET_SUBSCRIBE_COV_DATA * cov_data)
{
    BACNET_ADDRESS my_address;
    uint8_t invoke_id = 0;
    int len = 0;
    int pdu_len = 0;
    int bytes_sent = 0;
    BACNET_NPDU_DATA npdu_data;

    if (!dcc_communication_enabled())
        return 0;

    invoke_id = tsm_next_free_invokeID();
    if (invoke_id) {
        /* encode the NPDU portion of the packet */
        datalink_get_my_address(&my_address);
        npdu_encode_npdu_data(&npdu_data, true, MESSAGE_PRIORITY_NORMAL);
        pdu_len =
            npdu_encode_pdu(&Handler_Transmit_Buffer[0], dest, &my_address,
            &npdu_data);
        /* encode the APDU portion of the packet */
        len =
            cov_subscribe_encode_apdu(&Handler_Transmit_Buffer[pdu_len],
            invoke_id, cov_data);
        pdu_len += len;
        /* will it fit in the sender?
           note: if there is a bottleneck router in between
           us and the destination, we won't know unless
           we have a way to check for that and update the
           max_apdu in the address binding table. */
        if (pdu_len < max_apdu) {
            tsm_set_confirmed_unsegmented_transaction(invoke_id, &dest,
                &npdu_data, &Handler_Transmit_Buffer[0], (uint16_t) pdu_len);
            bytes_sent =
                datalink_send_pdu(&dest, &npdu_data,
                &Handler_Transmit_Buffer[0], pdu_len);
#if PRINT_ENABLED
            if (bytes_sent <= 0)
                fprintf(stderr, "Failed to Send SubscribeCOV Request (%s)!\n",
                    strerror(errno));
#endif
        } else {
            tsm_free_invoke_id(invoke_id);
            invoke_id = 0;
#if PRINT_ENABLED
            fprintf(stderr,
                "Failed to Send SubscribeCOV Request "
                "(exceeds destination maximum APDU)!\n");
#endif
        }
    }

    return invoke_id;
}
