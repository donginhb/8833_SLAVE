
#ifndef __SEND_TEST_DATA_FPGA_H__
#define __SEND_TEST_DATA_FPGA_H__

#pragma pack(1)
typedef struct
{
    uint16_t vol;//��ѹ
    uint16_t cur;//����
    uint16_t real;//��ʵ����
    uint8_t cur_inf;//������Ϣ
    uint8_t test_flag;//���Ա��
    uint8_t test_status;//����״̬
    uint16_t time;//����ʱ��
}ACW_FRAME;

typedef struct
{
    uint16_t vol;//��ѹ
    uint16_t cur;//����
    uint8_t cur_inf;//������Ϣ
    uint8_t test_flag;//���Ա��
    uint8_t test_status;//����״̬
    uint16_t time;//����ʱ��
}DCW_FRAME;

typedef struct
{
    uint16_t vol;//��ѹ
    uint16_t res;//����
    uint8_t res_inf;//������Ϣ
    uint8_t test_flag;//���Ա��
    uint8_t test_status;//����״̬
    uint16_t time;//����ʱ��
}IR_FRAME;

typedef struct
{
    uint16_t cur;//����
    uint16_t res;//����
    uint8_t res_inf;//������Ϣ
    uint8_t test_flag;//���Ա��
    uint8_t test_status;//����״̬
    uint16_t time;//����ʱ��
}GR_FRAME;

#pragma pack()


void send_slave_test_data_fpga(void);

#endif //__SEND_TEST_DATA_FPGA_H__
