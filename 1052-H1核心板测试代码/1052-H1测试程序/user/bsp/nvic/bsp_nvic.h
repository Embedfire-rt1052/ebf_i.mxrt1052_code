#ifndef __NVIC_H
#define __NVIC_H

#include "fsl_common.h"

/*定义中断优先级分组 */
typedef enum PriorityGroup{

  Group_0 = 0,
	Group_1,
	Group_2,
	Group_3,
  Group_4,
  Group_5,
  Group_6,
  Group_7,
}PriorityGroup_Type;

/*定义抢占优先级*/
typedef enum PreemptPriority {
  Group0_PreemptPriority_0 = 0,
  Group0_PreemptPriority_1,
  Group0_PreemptPriority_2,
  Group0_PreemptPriority_3,
  Group0_PreemptPriority_4,
  Group0_PreemptPriority_5,
  Group0_PreemptPriority_6,
  Group0_PreemptPriority_7,
  Group0_PreemptPriority_8,
  Group0_PreemptPriority_9,
  Group0_PreemptPriority_10,
  Group0_PreemptPriority_11,
  Group0_PreemptPriority_12,
  Group0_PreemptPriority_13,
  Group0_PreemptPriority_14,
  Group0_PreemptPriority_15, 
	
	
	
	Group1_PreemptPriority_0 = 0,
  Group1_PreemptPriority_1,
  Group1_PreemptPriority_2,
  Group1_PreemptPriority_3,
  Group1_PreemptPriority_4,
  Group1_PreemptPriority_5,
  Group1_PreemptPriority_6,
  Group1_PreemptPriority_7,
  Group1_PreemptPriority_8,
  Group1_PreemptPriority_9,
  Group1_PreemptPriority_10,
  Group1_PreemptPriority_11,
  Group1_PreemptPriority_12,
  Group1_PreemptPriority_13,
  Group1_PreemptPriority_14,
  Group1_PreemptPriority_15,

  Group2_PreemptPriority_0 = 0,
  Group2_PreemptPriority_1,
  Group2_PreemptPriority_2,
  Group2_PreemptPriority_3,
  Group2_PreemptPriority_4,
  Group2_PreemptPriority_5,
  Group2_PreemptPriority_6,
  Group2_PreemptPriority_7,
  Group2_PreemptPriority_8,
  Group2_PreemptPriority_9,
  Group2_PreemptPriority_10,
  Group2_PreemptPriority_11,
  Group2_PreemptPriority_12,
  Group2_PreemptPriority_13,
  Group2_PreemptPriority_14,
  Group2_PreemptPriority_15,

  Group3_PreemptPriority_0 = 0,
  Group3_PreemptPriority_1,
  Group3_PreemptPriority_2,
  Group3_PreemptPriority_3,
  Group3_PreemptPriority_4,
  Group3_PreemptPriority_5,
  Group3_PreemptPriority_6,
  Group3_PreemptPriority_7,
  Group3_PreemptPriority_8,
  Group3_PreemptPriority_9,
  Group3_PreemptPriority_10,
  Group3_PreemptPriority_11,
  Group3_PreemptPriority_12,
  Group3_PreemptPriority_13,
  Group3_PreemptPriority_14,
  Group3_PreemptPriority_15,



  Group4_PreemptPriority_0 = 0,
  Group4_PreemptPriority_2,
  Group4_PreemptPriority_3,
  Group4_PreemptPriority_4,
  Group4_PreemptPriority_5,
  Group4_PreemptPriority_6,
  Group4_PreemptPriority_7,
	
  
  Group5_PreemptPriority_0 = 0,
  Group5_PreemptPriority_1,
  Group5_PreemptPriority_2,
  Group5_PreemptPriority_3,
  
  Group6_PreemptPriority_0 = 0,
  Group6_PreemptPriority_1,
  
  Group7_PreemptPriority_0 = 0
}PreemptPriority_Type;

/*定义子优先级*/
typedef enum SubPriority {

  Group0_SubPriority_0 = 0,
	Group1_SubPriority_0 = 0,
	Group2_SubPriority_0 = 0,
	Group3_SubPriority_0 = 0,
			
  Group4_SubPriority_0 = 0,
  Group4_SubPriority_1,
  
  Group5_SubPriority_0 = 0,
  Group5_SubPriority_1,
  Group5_SubPriority_2,
  Group5_SubPriority_3,
  
  
  Group6_SubPriority_0 = 0,
  Group6_SubPriority_2,
  Group6_SubPriority_3,
  Group6_SubPriority_4,
  Group6_SubPriority_5,
  Group6_SubPriority_6,
  Group6_SubPriority_7,
  
  Group7_SubPriority_0 = 0,
  Group7_SubPriority_1,
  Group7_SubPriority_2,
  Group7_SubPriority_3,
  Group7_SubPriority_4,
  Group7_SubPriority_5,
  Group7_SubPriority_6,
  Group7_SubPriority_7,
  Group7_SubPriority_8,
  Group7_SubPriority_9,
  Group7_SubPriority_10,
  Group7_SubPriority_11,
  Group7_SubPriority_12,
  Group7_SubPriority_13,
  Group7_SubPriority_14,
  Group7_SubPriority_15
  
}SubPriority_Type;


void Set_NVIC_PriorityGroup(PriorityGroup_Type PriorityGroup) ;
void set_IRQn_Priority(IRQn_Type IRQn,PreemptPriority_Type PreemptPriority, SubPriority_Type SubPriorit);


#endif /* __NVIC_H */





