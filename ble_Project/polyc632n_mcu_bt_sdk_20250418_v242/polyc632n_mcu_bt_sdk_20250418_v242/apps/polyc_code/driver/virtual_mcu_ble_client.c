#include "virtual_mcu_bt_communication.h"
#include "virtual_mcu_main.h"
#include "virtual_mcu_ble_client.h"

static bool ble_client_connect_flag = 0;      ///ble_client�����Ƿ�������״̬
u8 curr_client_peer_addr_info[7];         //��ǰ���ӶԷ���ַ��Ϣ  1λaddr_type + 6λaddress
u8 curr_client_peer_device_name_info[29];  //��ǰ���ӶԷ��豸����Ϣ,���29���ֽ�

static u8 set_device_match_creat_by_address[7]={0}; //ָ����ַ��������  1λaddr_type + 6λaddress
static u8 set_device_match_creat_by_address_flag =0;

static u8 set_device_match_creat_by_name[29]={0};  //ָ���豸���ƴ�������
static u8 set_device_match_creat_by_name_valid_len =0; //ָ���豸������Ч����


//*����BLE����״̬�仯����   ע��:������ϵͳ�ײ�ص����벻Ҫ������������*/
static void mcu_ble_client_state_change_callback(u8 state, u8 *priv, u16 len)
{
    printf("-->client_state_change:0x%02x\n", state);
	/*
    if(priv && len){
        printf("client--->priv:\n");
        printf_buf(priv,len);
    }
	*/
    switch(state)
    {
        case POLYC_BLE_ST_INIT_OK:  //BLE����Э��ջ��ʼ��ok
            ble_client_connect_flag =0;  //����BLE�����ѶϿ�
            break;

        case POLYC_BLE_ST_DISCONN:
            printf("disconn_reason:0x%02x\n", priv[0]);
			memset(curr_client_peer_device_name_info, 0, sizeof(curr_client_peer_device_name_info)); //�嵱ǰ���ӶԷ��豸����Ϣ
            ble_client_connect_flag =0;  //����BLE�����ѶϿ�
            break;

        case POLYC_BLE_ST_CONNECT:
            memcpy(curr_client_peer_addr_info, priv, 7); //�浱ǰ���ӶԷ���ַ��Ϣ
            ble_client_connect_flag =1;  //����BLE����������
            printf("curr_client_peer_addr_info:\n");
            printf_buf(curr_client_peer_addr_info, 7);
            break;

        case POLYC_BLE_MTU_EXCHANGE_COMPLETE:
            break;
    }
}

//����Ƿ���ƥ����豸
static u8 mcu_ble_client_check_device_is_match(u8 type, u8 mode, u8 *data, u16 data_len)
{
    u8 check_ok = 0;

    if( (data == NULL) || (data_len == 0) ){ //����Ϊ0
        return 0;
    }

    switch(mode)
    {
    case POLYC_CLIENT_CLI_CREAT_BY_ADDRESS: //ָ����ַ��������
        if(set_device_match_creat_by_address_flag){
            if(set_device_match_creat_by_address[0] == type){ //��ַ����һ��
                if (0 == memcmp(data, &set_device_match_creat_by_address[1], 6)) { //��ַһ��
                    check_ok =1;
                }
            }
        }
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_NAME: //ָ���豸���ƴ�������
        if(set_device_match_creat_by_name_valid_len){
            if (0 == memcmp(data, set_device_match_creat_by_name, set_device_match_creat_by_name_valid_len)) {
                check_ok =1;
            }
        }
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_TAG: //ƥ�䳧�ұ�ʶ��������
        break;
    }

    return check_ok;
}

/*************************************************************************************************/
/*!
 *  \brief      ble_client��������scan����adv&rsp������
 *  \param      [in]
 *  \return     �Ƿ���ƥ����豸, true or false
 *  \note
 */
/*************************************************************************************************/
static bool ble_client_resolve_adv_report(polyc_adv_report_t *report_pt, u16 len)
{
    u8 i, length, ad_type;
    u8 *adv_data_pt;
    u8 find_remoter = 0;
    u32 tmp32;

    //printf(">>>event_type, addr_type: %x,%x; ",report_pt->event_type,report_pt->address_type);
	//printf_buf(report_pt->address,6);

	//printf("adv_data_display:%d", report_pt->length);
	//printf_buf(report_pt->data,report_pt->length);

	//printf("rssi:%d\n", report_pt->rssi);

    if (mcu_ble_client_check_device_is_match(report_pt->address_type, POLYC_CLIENT_CLI_CREAT_BY_ADDRESS, report_pt->address, 6)) {//ƥ�䵽ָ����ַ
        find_remoter = 1;
        printf("catch adderss ok!\n");
    }

    adv_data_pt = report_pt->data;
    for (i = 0; i < report_pt->length;) {
        if (*adv_data_pt == 0) {
            //printf("analyze end\n");
            break;
        }

        length = *adv_data_pt++;

        if (length >= report_pt->length || (length + i) >= report_pt->length) {
            /*���˷Ǳ�׼����ʽ*/
            //printf("!!!error_adv_packet:");
            //printf_buf(report_pt->data, report_pt->length);
            break;
        }

        ad_type = *adv_data_pt++;
        i += (length + 1);

        switch (ad_type) {
        case BLE_HCI_EIR_DATATYPE_FLAGS:
            /* printf("flags:%02x\n",adv_data_pt[0]); */
            break;

        case BLE_HCI_EIR_DATATYPE_MORE_16BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_COMPLETE_16BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_MORE_32BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_COMPLETE_32BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_MORE_128BIT_SERVICE_UUIDS:
        case BLE_HCI_EIR_DATATYPE_COMPLETE_128BIT_SERVICE_UUIDS:
            //printf("service uuid:");
            //printf_buf(adv_data_pt, length - 1);
            break;

        case BLE_HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME:
        case BLE_HCI_EIR_DATATYPE_SHORTENED_LOCAL_NAME:
            tmp32 = adv_data_pt[length - 1];
            adv_data_pt[length - 1] = 0;;
            printf("remoter_name:%s ,rssi:%d\n", adv_data_pt, report_pt->rssi);
            printf_buf(report_pt->address, 6);
            adv_data_pt[length - 1] = tmp32;

            if (mcu_ble_client_check_device_is_match(report_pt->event_type, POLYC_CLIENT_CLI_CREAT_BY_NAME, adv_data_pt, length - 1)) { //ƥ�䵽ָ���豸����
                find_remoter = 1;
                printf("catch name ok!\n");
            }
            break;

        case BLE_HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA:
            if (mcu_ble_client_check_device_is_match(report_pt->event_type, POLYC_CLIENT_CLI_CREAT_BY_TAG, adv_data_pt, length - 1)) {
                find_remoter = 1;
                printf("catch tag ok!\n");
            }
            break;

        case BLE_HCI_EIR_DATATYPE_APPEARANCE_DATA:
            /* printf("get_class_type:%04x\n",little_endian_read_16(adv_data_pt,0)); */
            break;

        default:
            /* printf("unknow ad_type:"); */
            break;
        }

        adv_data_pt += (length - 1);
    }

    return find_remoter;
}


//*����client����Э��ջ�ص���scan����adv&rsp ��   ע��:������ϵͳ�ײ�ص����벻Ҫ����������*/
static void mcu_ble_client_report_adv_data_callback(polyc_adv_report_t *report_pt, u16 len)
{
    bool find_tag = 0;

    if(get_virtual_mcu_ble_app_mode() == VIRTUAL_MCU_BLE_CLIENT_APP_MODE) //��BLE_CLIENT����Ӧ��ģʽ��
    {
        //putchar('c');

        find_tag = ble_client_resolve_adv_report(report_pt, len); //ble_client��������scan����adv&rsp������

        if(find_tag) //ɨ�赽ƥ����豸
        {
            u8 address_info[7];
            address_info[0] =report_pt->address_type; //�Է���ַ����
            memcpy(&address_info[1], report_pt->address, 6); //�Է���ַ
            polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_CONNECT, address_info, sizeof(address_info));  //BLE�����������Ӽ���
        }
    }
}

//ble_client���������ݽ��ջص�����
static void (*mcu_ble_client_data_receive_func_callback)(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size) =NULL;
void mcu_ble_client_data_receive_register_callback(void *cbk(u16 value_handle, u16 offset, u8 *buffer, u16 buffer_size))
{
    mcu_ble_client_data_receive_func_callback =cbk;
}

//*ble_client�������յ��ӻ�server�˵����ݻص�����   ע��:������ϵͳ�ײ�ص����벻Ҫ����������*/
static void mcu_ble_client_report_gatt_data_callback(polyc_att_data_report_t *report_data)
{
    //printf("packet_type:0x%04x, conn_handle:0x%04x, value_handle:0x%04x\n", report_data->packet_type, report_data->conn_handle, report_data->value_handle);
    //printf("value_offset:%d, blob_length=%d\n", report_data->value_offset, report_data->blob_length);
    //printf_buf(report_data->blob, report_data->blob_length);

    switch (report_data->packet_type)
    {
    case 0xA7://notify
        if(mcu_ble_client_data_receive_func_callback){
            mcu_ble_client_data_receive_func_callback(report_data->value_handle, report_data->value_offset, report_data->blob, report_data->blob_length);
        }
        break;

    case 0xA8://indicate
        break;

    case 0xA5://read
        if(report_data->blob_length > sizeof(curr_client_peer_device_name_info) ){
            report_data->blob_length =sizeof(curr_client_peer_device_name_info);
        }
        memcpy(curr_client_peer_device_name_info, report_data->blob, report_data->blob_length); //�浱ǰ���ӶԷ��豸����Ϣ
        printf("peer_device_name:%s\n", curr_client_peer_device_name_info);
        break;

    case 0xA6://read long
        break;

    default:
        break;
    }
}

// ble_client�����˷����ӿ�  ����0,���ͳɹ�
int mcu_ble_client_data_send(u8 *buff, u16 buff_size)
{
    return polyc_ble_client_data_send(buff, buff_size, MCU_BLE_ATT_OP_WRITE_WITHOUT_RESPOND);
}


 //����BLE������������ָ��uuid
static void mcu_ble_client_set_search_uuid_table(void)
{
    //��ȡԶ��server�豸��device_name
    client_search_uuid_table[0].services_uuid16 =0x1800;
    client_search_uuid_table[0].characteristic_uuid16 =0x2a00;
    client_search_uuid_table[0].opt_type =POLYC_ATT_PROPERTY_READ;

    //UUID16_NOTIFY    ��������ͨ��:ble_client��������Զ��BLE�ӻ�������ͨ��
    client_search_uuid_table[1].services_uuid16 =0xFFF0;
    client_search_uuid_table[1].characteristic_uuid16 =0xFFF1;
    client_search_uuid_table[1].opt_type =POLYC_ATT_PROPERTY_NOTIFY;

    //UUID16_WRITE    ��������ͨ��:ble_client�������͸�Զ��BLE�ӻ�������ͨ��
    client_search_uuid_table[2].services_uuid16 =0xFFF0;
    client_search_uuid_table[2].characteristic_uuid16 =0xFFF2;
    client_search_uuid_table[2].opt_type =POLYC_ATT_PROPERTY_WRITE_WITHOUT_RESPONSE;
}

//ble_client������ʼ������
void mcu_ble_client_init(void)
{
	printf("--> mcu_ble_client_init\n");

    polyc_ble_client_state_change_register_callback(mcu_ble_client_state_change_callback); //ע��ble_client����״̬�仯����

    polyc_ble_client_report_adv_data_register_callback(mcu_ble_client_report_adv_data_callback); //ע��ble_client�����㲥�����ݽ��ջص�����
    polyc_ble_client_report_gatt_data_register_callback(mcu_ble_client_report_gatt_data_callback); //ע������client���յ��ӻ�server�˵����ݻص�����

    polyc_ble_client_scan_config_set(SET_TYPE_SCAN_ACTIVE, MCU_BLE_CLIENT_SET_SCAN_INTERVAL, MCU_BLE_CLIENT_SET_SCAN_WINDOW);  //BLE����ɨ���������
    polyc_ble_client_conn_config_set(MCU_BLE_CLIENT_SET_CONN_INTERVAL, MCU_BLE_CLIENT_SET_CONN_LATENCY, MCU_BLE_CLIENT_SET_CONN_TIMEOUT);  //BLE�������Ӳ�������

    mcu_ble_client_set_search_uuid_table(); //����BLE��������ָ������uuid
}

void mcu_ble_client_set_device_match(u8 creat_mode, u8 *data, u16 data_len) //����BLE��������ָ���豸
{
    printf("client_set_device_match:%d\n", creat_mode);

    switch(creat_mode)
    {
    case POLYC_CLIENT_CLI_CREAT_BY_ADDRESS: //ָ����ַ��������
        memset(&set_device_match_creat_by_address, 0, sizeof(set_device_match_creat_by_address));

        if( (data_len == 0) || (data == NULL) ){//��ָ����ַ�������ӱ�־
            set_device_match_creat_by_address_flag =0;
            break;
        }

        if(data_len > sizeof(set_device_match_creat_by_address)){
           data_len =sizeof(set_device_match_creat_by_address);
        }
        memcpy(&set_device_match_creat_by_address, data, data_len); //1λ��ַ���� + 6λmac��ַ
        set_device_match_creat_by_address_flag =1; //����ָ����ַ�������ӱ�־
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_NAME: //ָ���豸���ƴ�������
        memset(&set_device_match_creat_by_name, 0, sizeof(set_device_match_creat_by_name));

        if( (data_len == 0) || (data == NULL) ){//��ָ���豸���ƴ������ӱ�־
            set_device_match_creat_by_name_valid_len =0;
            break;
        }

        if(data_len > (sizeof(set_device_match_creat_by_name)-1) ){
           data_len =sizeof(set_device_match_creat_by_name)-1; //ȥ������
        }
        memcpy(&set_device_match_creat_by_name, data, data_len); //��ָ���豸��
        set_device_match_creat_by_name_valid_len =data_len; //��ָ���豸���ַ�����
        break;

    case POLYC_CLIENT_CLI_CREAT_BY_TAG: //ƥ�䳧�ұ�ʶ��������
        break;
    }
}

//ble_client������ȡ��·�Է����ź�ǿ��, ��Χ-127 ~ 127
s8 mcu_get_ble_client_curr_rssi(void)
{
    u16 get_ble_cmd_len =0;
    s8 curr_rssi =0;
    polyc_get_ble_cmd(MCU_GET_BLE_CLIENT_PEER_RSSI, &curr_rssi, get_ble_cmd_len);

    //printf("client_rssi:%d\n",curr_rssi);
    return curr_rssi;
}

//��ȡble_client��������״̬
u8 mcu_get_ble_client_connect_flag(void)
{
    return ble_client_connect_flag;
}

//�Ͽ�ble_client��������
void mcu_ble_client_disconnected(void)
{
    polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_DISCONN, NULL, 0);
}

///BLE_CLIENT����Ӧ��ģʽ�����͹ر�  1:����  0:�ر�
void mcu_ble_client_enable(u8 enable)
{
    if(enable){
        if(get_virtual_mcu_ble_app_mode() != VIRTUAL_MCU_BLE_CLIENT_APP_MODE){ //��ǰ����BLE_CLIENT����Ӧ��ģʽ
            polyc_ble_client_report_adv_data_register_callback(mcu_ble_client_report_adv_data_callback); //ע��ble_client�����㲥�����ݽ��ջص�����
            polyc_ble_client_scan_config_set(SET_TYPE_SCAN_ACTIVE, MCU_BLE_CLIENT_SET_SCAN_INTERVAL, MCU_BLE_CLIENT_SET_SCAN_WINDOW);  //BLE����ɨ���������
            polyc_ble_client_conn_config_set(MCU_BLE_CLIENT_SET_CONN_INTERVAL, MCU_BLE_CLIENT_SET_CONN_LATENCY, MCU_BLE_CLIENT_SET_CONN_TIMEOUT);  //BLE�������Ӳ�������

            polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_ON, NULL, 0); //BLE_CLIENT����Ӧ��ģʽ��
        }
    }else{
        polyc_set_ble_cmd(MCU_SET_BLE_CLIENT_OFF, NULL, 0); //BLE_CLIENT����Ӧ��ģʽ�ر�
    }
}


