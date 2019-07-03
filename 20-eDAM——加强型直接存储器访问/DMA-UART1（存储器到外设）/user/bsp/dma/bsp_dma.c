#include "fsl_edma.h"
#include "fsl_dmamux.h"

#include "./dma/bsp_dma.h"   


edma_handle_t g_EDMA_Handle;//����eDAM������
volatile bool g_Transfer_Done = false;//���崫����ɱ�־
AT_NONCACHEABLE_SECTION_INIT(uint32_t srcAddr[BUFF_LENGTH]) = {0x01, 0x02, 0x03, 0x04};//Դ���ݻ�����
AT_NONCACHEABLE_SECTION_INIT(uint32_t destAddr[BUFF_LENGTH]) = {0x00, 0x00, 0x00, 0x00};//Ŀ�����ݻ�����



void DMA_Config(void)
{
  edma_transfer_config_t transferConfig;//���� edma �������ýṹ��
  edma_config_t userConfig;//���� edma ȫ�����ýṹ��

  /* ��ʼ��DMAMUX */
  DMAMUX_Init(EXAMPLE_DMAMUX);
  
  /*����DMA ͨ��������һֱ����*/
  DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX, eDAM_Channel, true); 
  /*ʹ��ͨ��*/
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, eDAM_Channel);
  /*���Ĭ������
  * userConfig.enableRoundRobinArbitration = false; ѡ��ʹ��ѭ��ͨ���ٲ�ģʽ��
  * userConfig.enableHaltOnError = true;            ʹ�ܴ���ֹͣ
  * userConfig.enableContinuousLinkMode = false;    ��ֹ��������ģʽ
  * userConfig.enableDebugMode = false;             ��ֹDEBUGģʽ
   */
  EDMA_GetDefaultConfig(&userConfig);
  /*��ʼ��eDAM*/
  EDMA_Init(EXAMPLE_DMA,&userConfig);
  
  /*����eDMA���*/
  EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, eDAM_Channel);
  /*���ô�����ɻص�����*/
  EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
  
  /*eDMAԤ���䣬�������ò�����ɴ������ýṹ��ĳ�ʼ��*/
  EDMA_PrepareTransfer(&transferConfig,\
                       srcAddr,\
                       sizeof(srcAddr[0]),\
                       destAddr,\
                       sizeof(destAddr[0]),\
                       sizeof(srcAddr[0]),\
                       sizeof(srcAddr),\
                       kEDMA_MemoryToMemory);         

//  EDMA_PrepareTransfer(&transferConfig, \             //�������ýṹ��           
//                       srcAddr, \                     //Դ��ַ
//                       sizeof(srcAddr[0]), \          //Դ���ݿ��
//                       destAddr, \                    //Ŀ�ĵ�ַ
//                       sizeof(destAddr[0]),\          //Ŀ�ĵ�ַ���
//                       sizeof(srcAddr[0]),\           //ÿ��DMA��������ֽ���
//                       sizeof(srcAddr), \             //����������
//                       kEDMA_MemoryToMemory);         //��������
  
  
  
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

