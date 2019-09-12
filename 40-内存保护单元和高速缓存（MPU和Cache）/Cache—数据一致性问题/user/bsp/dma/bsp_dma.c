#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "./dma/bsp_dma.h"   


edma_handle_t g_EDMA_Handle;            //����eDAM������
volatile bool g_Transfer_Done = false;//���崫����ɱ�־
uint32_t srcAddr[BUFF_LENGTH] = {0x01, 0x02, 0x03, 0x04};//Դ���ݻ�����
uint32_t destAddr[BUFF_LENGTH] = {0x00, 0x00, 0x00, 0x00};//Ŀ�����ݻ�����



void DMA_Config(void)
{
  edma_transfer_config_t transferConfig;//���� edma �������ýṹ��
  edma_config_t userConfig;//���� edma ȫ�����ýṹ��

  /* ��ʼ��DMAMUX */
  DMAMUX_Init(EXAMPLE_DMAMUX);
  /*����DMA ͨ��һֱ���ڻ״̬*/
  DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX, eDAM_Channel, true); 
  /*ʹ��ͨ��*/
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, eDAM_Channel);
  
  
  /*���Ĭ������
  * userConfig.enableRoundRobinArbitration = false; ʹ�ù̶����ȼ�ģʽ��
  * userConfig.enableHaltOnError = true;            ʹ�ܴ���ֹͣ
  * userConfig.enableContinuousLinkMode = false;    ��ʹ�ô�ѭ��ͨ������
  * userConfig.enableDebugMode = false;             ��ֹDEBUGģʽ����Debug ģʽ��DMA�������У�
   */
  EDMA_GetDefaultConfig(&userConfig);
  /*��ʼ��eDMA*/
  EDMA_Init(EXAMPLE_DMA,&userConfig);
  
  /*��ʼ���������ýṹ��*/
  transferConfig.srcAddr = (uint32_t)srcAddr;  //Դ��ַ       
  transferConfig.srcOffset = 4;                //Դ��ַƫ��
  transferConfig.srcTransferSize = kEDMA_TransferSize4Bytes;//Դ���ݿ��
  
  transferConfig.destAddr = (uint32_t)destAddr;//Ŀ�ĵ�ַ
  transferConfig.destOffset = 4;               //Ŀ�ĵ�ַƫ��
  transferConfig.destTransferSize = kEDMA_TransferSize4Bytes;//Ŀ�����ݿ��
  
  transferConfig.minorLoopBytes = 16;          //��ѭ�������ֽ���
  transferConfig.majorLoopCounts = 1;          //��ѭ��ִ�д���
  
  /*����eDMA���*/
  EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, eDAM_Channel);
  /*���ô�����ɻص�����*/
  EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
  /*�ύeDAM��������*/
  EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);        
  /*��������*/
  EDMA_StartTransfer(&g_EDMA_Handle);  
}


/*  EDMA ������ɻص�����. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}
