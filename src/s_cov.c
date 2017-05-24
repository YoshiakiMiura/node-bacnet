#include "cov.h"

int Send_UCOV_Notify_Address(
    BACNET_ADDRESS dest,
    uint8_t * buffer,
    BACNET_COV_DATA * cov_data)
{
    int pdu_len = 0;
    BACNET_ADDRESS dest;
    int bytes_sent = 0;
    BACNET_NPDU_DATA npdu_data;

    pdu_len = ucov_notify_encode_pdu(buffer, &dest, &npdu_data, cov_data);
    bytes_sent = datalink_send_pdu(&dest, &npdu_data, &buffer[0], pdu_len);

    return bytes_sent;
}
