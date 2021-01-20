#include "sdk_common.h"
#include "ble_cus.h"
#include <string.h>
#include "ble_srv_common.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_cus_t * p_cus, ble_evt_t const * p_ble_evt)
{
    p_cus->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

    ble_cus_evt_t evt;

    evt.evt_type = BLE_CUS_EVT_CONNECTED;

    p_cus->evt_handler(p_cus, &evt);
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_cus_t * p_cus, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_cus->conn_handle = BLE_CONN_HANDLE_INVALID;
    
    ble_cus_evt_t evt;

    evt.evt_type = BLE_CUS_EVT_DISCONNECTED;

    p_cus->evt_handler(p_cus, &evt);
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_cus_t * p_cus, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
        //    char *p=&p_ble_evt->evt.gatts_evt.params.write.handle;
        //printf("0x%02X%02X\n",*(p+1),*p);
        //printf("%d\n",p_evt_write->data);
        //printf("%s\n",p_evt_write->data);
    // Custom Value Characteristic Written to.
    if (p_evt_write->handle == p_cus->custom_value_handles.value_handle)
    {
       /* nrf_gpio_pin_toggle(LED_4);

        if(*p_evt_write->data == 0x01)
        {
            nrf_gpio_pin_clear(20); 
        }
        else if(*p_evt_write->data == 0x04)
        {
            nrf_gpio_pin_set(20); 
        }
        else
        {
          //Do nothing
        }*/
        
    }

    // Check if the Custom value CCCD is written to and that the value is the appropriate length, i.e 2 bytes.
    if ((p_evt_write->handle == p_cus->custom_value_handles.cccd_handle)
        && (p_evt_write->len == 2)
       )
    {
        // CCCD written, call application event handler
        if (p_cus->evt_handler != NULL)
        {
            ble_cus_evt_t evt;

            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_CUS_EVT_NOTIFICATION_ENABLED;
                printf("enable notify\n");
            }
            else
            {
                evt.evt_type = BLE_CUS_EVT_NOTIFICATION_DISABLED;
                printf("disable notify\n");

            }
            // Call the application event handler.
            p_cus->evt_handler(p_cus, &evt);
        }
    }

}

void ble_cus_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_cus_t * p_cus = (ble_cus_t *) p_context;
    printf("###########################\n");
    NRF_LOG_INFO("BLE event received. Event type = %d\r\n", p_ble_evt->header.evt_id); 
    printf("BLE event received. Event type = 0x%02X \r\n", p_ble_evt->header.evt_id); 
    if (p_cus == NULL || p_ble_evt == NULL)
    {
        printf("error\n");
        return;
    }
       
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_cus, p_ble_evt);
            printf("###########BLE_GAP_EVT_CONNECTED#########\n");
            break;

        case BLE_GAP_EVT_DISCONNECTED:            
            printf("##########BLE_GAP_EVT_DISCONNECTED##########\n");
            on_disconnect(p_cus, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            printf("##########BLE_GATTS_EVT_WRITE##########\n");
            on_write(p_cus, p_ble_evt);
            break;
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            NRF_LOG_INFO("EXCHANGE_MTU_REQUEST event received.\r\n");
            printf("##########EXCHANGE_MTU_REQUEST##########\n");
            break;
        case SD_BLE_GATTS_SERVICE_ADD:
            printf("##########SD_BLE_GATTS_SERVICE_ADD##########\n");
            break;    
        case SD_BLE_GATTS_INCLUDE_ADD:
            printf("##########SD_BLE_GATTS_INCLUDE_ADD##########\n");
            break;
        case SD_BLE_GATTS_CHARACTERISTIC_ADD:
            printf("##########SD_BLE_GATTS_CHARACTERISTIC_ADD##########\n");
            break;
        case SD_BLE_GATTS_DESCRIPTOR_ADD:
            printf("##########SD_BLE_GATTS_DESCRIPTOR_ADD##########\n");
            break;
        case SD_BLE_GATTS_VALUE_SET:
            printf("##########SD_BLE_GATTS_VALUE_SET##########\n");
            break;    
        case SD_BLE_GATTS_VALUE_GET:
            printf("##########SD_BLE_GATTS_VALUE_GET##########\n");
            break;
        case SD_BLE_GATTS_HVX:
            printf("##########SD_BLE_GATTS_HVX##########\n");
            break;
        case SD_BLE_GATTS_SERVICE_CHANGED:
            printf("##########SD_BLE_GATTS_SERVICE_CHANGED##########\n");
            break;
        case SD_BLE_GATTS_RW_AUTHORIZE_REPLY:
            printf("##########SD_BLE_GATTS_RW_AUTHORIZE_REPLY##########\n");
            break;
        case SD_BLE_GATTS_SYS_ATTR_SET:
            printf("##########SD_BLE_GATTS_SYS_ATTR_SET##########\n");
            break;    
        case SD_BLE_GATTS_SYS_ATTR_GET:
            printf("##########SD_BLE_GATTS_SYS_ATTR_GET##########\n");
            break;
        case SD_BLE_GATTS_INITIAL_USER_HANDLE_GET:
            printf("##########SD_BLE_GATTS_INITIAL_USER_HANDLE_GET##########\n");
            break;
        case SD_BLE_GATTS_ATTR_GET:
            printf("##########SD_BLE_GATTS_ATTR_GET##########\n");
            break;
        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            printf("##########BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST##########\n");
            break;    
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            printf("##########BLE_GATTS_EVT_SYS_ATTR_MISSING##########\n");
            break;
        case BLE_GATTS_EVT_HVC:
            printf("##########BLE_GATTS_EVT_HVC##########\n");
            break;
        case BLE_GATTS_EVT_SC_CONFIRM:
            printf("##########BLE_GATTS_EVT_SC_CONFIRM##########\n");
            break;
        case SD_BLE_GATTS_EXCHANGE_MTU_REPLY:
            printf("##########SD_BLE_GATTS_EXCHANGE_MTU_REPLY##########\n");
            break;
        case BLE_GATTS_EVT_TIMEOUT:
            printf("##########BLE_GATTS_EVT_TIMEOUT##########\n");
            break;
        case BLE_GATTS_EVT_HVN_TX_COMPLETE:
            printf("##########BLE_GATTS_EVT_HVN_TX_COMPLETE##########\n");
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            printf("##########BLE_GAP_EVT_CONN_PARAM_UPDATE##########\n");
            break;
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            printf("##########BLE_GAP_EVT_SEC_PARAMS_REQUEST##########\n");
            break;
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            printf("##########BLE_GAP_EVT_SEC_INFO_REQUEST##########\n");
            break;
        case BLE_GAP_EVT_PASSKEY_DISPLAY:
            printf("##########BLE_GAP_EVT_PASSKEY_DISPLAY##########\n");
            break;
        case BLE_GAP_EVT_KEY_PRESSED:
            printf("##########BLE_GAP_EVT_KEY_PRESSED##########\n");
            break;
        case BLE_GAP_EVT_AUTH_KEY_REQUEST:
            printf("##########BLE_GAP_EVT_AUTH_KEY_REQUEST##########\n");
            break;
        case BLE_GAP_EVT_LESC_DHKEY_REQUEST:
            printf("##########BLE_GAP_EVT_LESC_DHKEY_REQUEST##########\n");
            break;
        case BLE_GAP_EVT_AUTH_STATUS:
            printf("##########BLE_GAP_EVT_AUTH_STATUS##########\n");
            break;
        case BLE_GAP_EVT_CONN_SEC_UPDATE:
            printf("##########BLE_GAP_EVT_CONN_SEC_UPDATE##########\n");
            break;
        case BLE_GAP_EVT_TIMEOUT:
            printf("##########BLE_GAP_EVT_TIMEOUT##########\n");
            break;
        case BLE_GAP_EVT_RSSI_CHANGED:
            printf("##########BLE_GAP_EVT_RSSI_CHANGED##########\n");
            break;
        case BLE_GAP_EVT_SEC_REQUEST:
            printf("##########BLE_GAP_EVT_SEC_REQUEST##########\n");
            break;
        case BLE_GAP_EVT_SCAN_REQ_REPORT:
            printf("##########BLE_GAP_EVT_SCAN_REQ_REPORT##########\n");
            break;
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
            printf("##########BLE_GAP_EVT_PHY_UPDATE_REQUEST##########\n");
            break;
        case BLE_GAP_EVT_PHY_UPDATE:
            printf("##########BLE_GAP_EVT_PHY_UPDATE##########\n");
            break;
        case BLE_GAP_EVT_ADV_SET_TERMINATED:
            printf("##########BLE_GAP_EVT_ADV_SET_TERMINATED##########\n");
            break;
        case BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT:
            printf("##########BLE_GAP_OPT_AUTH_PAYLOAD_TIMEOUT##########\n");
            break;
        case BLE_GAP_OPT_SLAVE_LATENCY_DISABLE:
            printf("##########BLE_GAP_OPT_SLAVE_LATENCY_DISABLE##########\n");
            break;
        case BLE_GAP_CFG_DEVICE_NAME:
            printf("##########BLE_GAP_CFG_DEVICE_NAME##########\n");
            break;
        case BLE_GAP_TX_POWER_ROLE_ADV:
            printf("##########BLE_GAP_TX_POWER_ROLE_ADV##########\n");
            break;
        case BLE_GAP_TX_POWER_ROLE_CONN:
            printf("##########BLE_GAP_TX_POWER_ROLE_CONN##########\n");
            break;
        default:
            printf("##########other##########\n");
            // No implementation needed.
            break;
    }
        
        char *p=&p_ble_evt->evt.gatts_evt.params.write.handle;
        printf("handle : 0x%02X%02X\n",*(p+1),*p);
        char *p1=&p_ble_evt->evt.gatts_evt.params.write.data;
        printf("data :0x%02X%02X\n",*(p1+1),*p1);
        //printf("data :%s\n",p_ble_evt->evt.gatts_evt.params.write.data);
        //printf("size :%d\n",sizeof(p_ble_evt->evt.gatts_evt.params.write.data));
        int i=0;
        for (;*(p1+i)!='\0';i++)
        {
        printf("%c",*(p1+i));
        }
        printf("\n size %d",i);
}

/**@brief Function for adding the Custom Value characteristic.
 *
 * @param[in]   p_cus        Battery Service structure.
 * @param[in]   p_cus_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t custom_value_char_add(ble_cus_t * p_cus, const ble_cus_init_t * p_cus_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    // Add Custom Value characteristic
    memset(&cccd_md, 0, sizeof(cccd_md));

    //  Read  operation on cccd should be possible without authentication.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    
    cccd_md.write_perm = p_cus_init->custom_value_char_attr_md.cccd_write_perm;
    cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1; 
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md; 
    char_md.p_sccd_md         = NULL;
		
    ble_uuid.type = p_cus->uuid_type;
    ble_uuid.uuid = CUSTOM_VALUE_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_cus_init->custom_value_char_attr_md.read_perm;
    attr_md.write_perm = p_cus_init->custom_value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint8_t)*500;

    err_code = sd_ble_gatts_characteristic_add(p_cus->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_cus->custom_value_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

uint32_t ble_cus_init(ble_cus_t * p_cus, const ble_cus_init_t * p_cus_init)
{
    if (p_cus == NULL || p_cus_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_cus->evt_handler               = p_cus_init->evt_handler;
    p_cus->conn_handle               = BLE_CONN_HANDLE_INVALID;

    // Add Custom Service UUID
    ble_uuid128_t base_uuid = {CUSTOM_SERVICE_UUID_BASE};
    err_code =  sd_ble_uuid_vs_add(&base_uuid, &p_cus->uuid_type);
    VERIFY_SUCCESS(err_code);
    
    ble_uuid.type = p_cus->uuid_type;
    ble_uuid.uuid = CUSTOM_SERVICE_UUID;

    // Add the Custom Service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_cus->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Custom Value characteristic
    return custom_value_char_add(p_cus, p_cus_init);
}

uint32_t ble_cus_custom_value_update(ble_cus_t * p_cus, uint8_t custom_value)
{
    NRF_LOG_INFO("In ble_cus_custom_value_update. \r\n");
  printf("In ble_cus_custom_value_update with %d. \r\n",custom_value); 
    if (p_cus == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;
      uint16_t len = 27;
      uint8_t data[27] = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9}; // 20 bytes of data

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = &custom_value;

    //gatts_value.len     = sizeof(uint8_t);
    //gatts_value.offset  = 0;
    //gatts_value.p_value = &custom_value;
    uint16_t hh=0x10;
    // Update database.
    err_code = sd_ble_gatts_value_set(p_cus->conn_handle,
                                      hh,
                                      &gatts_value);

    if (err_code != NRF_SUCCESS)
    {

        return err_code;
    }

    // Send value if connected and notifying.
    if ((p_cus->conn_handle != BLE_CONN_HANDLE_INVALID)) 
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_cus->custom_value_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;



//hvx_params.handle = p_cus->conn_handle;
//hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
//hvx_params.offset = 0;
//hvx_params.p_len  = &len; // remember that this is also a pointer!
//hvx_params.p_data = data;

err_code = sd_ble_gatts_hvx(p_cus->conn_handle, &hvx_params);

       // err_code = sd_ble_gatts_hvx(p_cus->conn_handle, &hvx_params);
        NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code); 

    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
        NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");

    }


    return err_code;
}