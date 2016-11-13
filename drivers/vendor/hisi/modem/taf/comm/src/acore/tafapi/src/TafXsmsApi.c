

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafAppXsmsInterface.h"
#include "MnClient.h"



#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

#define THIS_FILE_ID                    PS_FILE_ID_TAF_XSMS_API_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/
/*****************************************************************************
�� �� ��  : TAF_XSMS_GetReceiverPid
��������  : ��Client ID������PID��ת��
�������  : ClientId -- AT�ͻ�ID
�������  : pulReceiverPid -- ����PID
�� �� ֵ  : VOS_OK/VOS_ERR
���ú���  : ��
��������  :

�޸���ʷ      :
 1.��    ��   : 2014��10��31��
   ��    ��   : h00300778
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_XSMS_GetReceiverPid(MN_CLIENT_ID_T  ClientId, VOS_UINT32 *pulReceiverPid)
{
#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
    MODEM_ID_ENUM_UINT16    enModemID;

    /* ���ýӿڻ�ȡModem ID */
    if (VOS_OK != AT_GetModemIdFromClient(ClientId, &enModemID))
    {
        return VOS_ERR;
    }

    if (MODEM_ID_1 == enModemID)
    {
        *pulReceiverPid = I1_UEPS_PID_XSMS;
    }
#if (MULTI_MODEM_NUMBER == 3)
    else if (MODEM_ID_2 == enModemID)
    {
        *pulReceiverPid = I2_UEPS_PID_XSMS;
    }
#endif /* MULTI_MODEM_NUMBER == 3 */
    else
    {
        *pulReceiverPid = I0_UEPS_PID_XSMS;
    }
#else
    *pulReceiverPid = UEPS_PID_XSMS;
#endif

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : TAF_XSMS_SendSmsReq
 ��������  : APP�Ķ��ŷ�������
 �������  : usClientId:����������Client��ID
             ucOpId:��ʶ���β���
             enSndOption:����ѡ��
             pucData:Ҫ���͵Ķ�������
 �������  : ��
 �� �� ֵ  : VOS_OK -- ��Ϣ���ͳɹ�
             VOS_ERR -- ��Ϣ���ͳɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��31��
    ��    ��   : h00300778
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_XSMS_SendSmsReq(
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId,
    TAF_XSMS_SEND_OPTION_ENUM_UINT8     enSndOption,
    VOS_UINT8                          *pucData)
{
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    TAF_XSMS_SEND_MSG_REQ_STRU         *pstMsg;
    VOS_UINT32                          ulReceiverPid;

    if (VOS_OK != TAF_XSMS_GetReceiverPid(usClientId, &ulReceiverPid))
    {
        return VOS_ERR;
    }

    if (VOS_NULL_PTR == pucData)
    {
        return VOS_ERR;
    }

    pstMsg = (TAF_XSMS_SEND_MSG_REQ_STRU *)VOS_AllocMsg(WUEPS_PID_AT, sizeof(TAF_XSMS_SEND_MSG_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    pstMsg->ulMsgName     = TAF_XSMS_APP_MSG_TYPE_SEND_REQ;
    pstMsg->ulReceiverPid = ulReceiverPid;
    pstMsg->usClientId    = usClientId;
    pstMsg->ucOpId        = ucOpId;
    pstMsg->enSndOption   = enSndOption;

    VOS_MemCpy(&pstMsg->st1XSms, pucData, sizeof(TAF_XSMS_MESSAGE_STRU));

    if (VOS_OK == VOS_SendMsg(WUEPS_PID_AT, pstMsg))
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
#else
    return VOS_ERR;
#endif
}

/*****************************************************************************
 �� �� ��  : TAF_XSMS_SetXsmsApMemFullReq
 ��������  : ֪ͨ����ģ�� AP����Ŵ洢�ռ��ǲ������� 0 �� 1 ����
 �������  : usClientId:����������Client��ID
             ucOpId:��ʶ���β���
             ucApMemFullFlag:���Ŵ洢�ռ��ǲ������� 0 �� 1 ����s
 �������  : ��
 �� �� ֵ  : VOS_OK -- ��Ϣ���ͳɹ�
             VOS_ERR -- ��Ϣ���ͳɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2015��06��02��
    ��    ��   : c00299064
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_XSMS_SetXsmsApMemFullReq(
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId,
    VOS_UINT8                           ucApMemFullFlag)
{
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    TAF_XSMS_APP_MSG_SET_AP_MEM_FULL_REQ_STRU               *pstMsg;
    VOS_UINT32                                               ulReceiverPid;

    if (VOS_OK != TAF_XSMS_GetReceiverPid(usClientId, &ulReceiverPid))
    {
        return VOS_ERR;
    }

    if (TAF_XSMS_AP_MEM_BUTT <= ucApMemFullFlag)
    {
        return VOS_ERR;
    }

    pstMsg = (TAF_XSMS_APP_MSG_SET_AP_MEM_FULL_REQ_STRU *)VOS_AllocMsg(WUEPS_PID_AT,
              sizeof(TAF_XSMS_APP_MSG_SET_AP_MEM_FULL_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    pstMsg->ulMsgName       = TAF_XSMS_APP_MSG_TYPE_UIM_MEM_SET_REQ;
    pstMsg->ulReceiverPid   = ulReceiverPid;
    pstMsg->usClientId      = usClientId;
    pstMsg->ucOpId          = ucOpId;
    pstMsg->enApMemFullFlag = ucApMemFullFlag;

    if (VOS_OK == VOS_SendMsg(WUEPS_PID_AT, pstMsg))
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
#else
    return VOS_OK;
#endif
}


/*****************************************************************************
 �� �� ��  : TAF_XSMS_WriteSmsReq
 ��������  : APP�Ķ���д������
 �������  : usClientId:����������Client��ID
             ucOpId:��ʶ���β���
             enStatus:����״̬
             pucData:Ҫ���͵Ķ�������
 �������  : ��
 �� �� ֵ  : VOS_OK -- ��Ϣ���ͳɹ�
             VOS_ERR -- ��Ϣ���ͳɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��31��
    ��    ��   : h00300778
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_XSMS_WriteSmsReq(
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId,
    TAF_XSMS_STATUS_ENUM_UINT8          enStatus,
    VOS_UINT8                          *pucData)
{
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    TAF_XSMS_WRITE_MSG_REQ_STRU        *pstMsg;
    VOS_UINT32                          ulReceiverPid;

    if (VOS_OK != TAF_XSMS_GetReceiverPid(usClientId, &ulReceiverPid))
    {
        return VOS_ERR;
    }

    if (VOS_NULL_PTR == pucData)
    {
        return VOS_ERR;
    }

    pstMsg = (TAF_XSMS_WRITE_MSG_REQ_STRU *)VOS_AllocMsg(WUEPS_PID_AT, sizeof(TAF_XSMS_WRITE_MSG_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    pstMsg->ulMsgName     = TAF_XSMS_APP_MSG_TYPE_WRITE_REQ;
    pstMsg->ulReceiverPid = ulReceiverPid;
    pstMsg->usClientId    = usClientId;
    pstMsg->ucOpId        = ucOpId;
    pstMsg->enSmsStatus   = enStatus;

    VOS_MemCpy(&pstMsg->st1XSms, pucData, sizeof(TAF_XSMS_MESSAGE_STRU));

    if (VOS_OK == VOS_SendMsg(WUEPS_PID_AT, pstMsg))
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
#else
    return VOS_ERR;
#endif
}

/*****************************************************************************
 �� �� ��  : TAF_XSMS_DeleteSmsReq
 ��������  : APP�Ķ���ɾ������
 �������  : usClientId:����������Client��ID
             ucOpId:��ʶ���β���
             ucIndex:ɾ����������
 �������  : ��
 �� �� ֵ  : VOS_OK -- ��Ϣ���ͳɹ�
             VOS_ERR -- ��Ϣ���ͳɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��10��31��
    ��    ��   : h00300778
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_XSMS_DeleteSmsReq(
    MN_CLIENT_ID_T                      usClientId,
    MN_OPERATION_ID_T                   ucOpId,
    VOS_UINT8                           ucIndex)
{
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    TAF_XSMS_DELETE_MSG_REQ_STRU       *pstMsg;
    VOS_UINT32                          ulReceiverPid;

    if (VOS_OK != TAF_XSMS_GetReceiverPid(usClientId, &ulReceiverPid))
    {
        return VOS_ERR;
    }

    pstMsg = (TAF_XSMS_DELETE_MSG_REQ_STRU *)VOS_AllocMsg(WUEPS_PID_AT, sizeof(TAF_XSMS_DELETE_MSG_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    pstMsg->ulMsgName     = TAF_XSMS_APP_MSG_TYPE_DELETE_REQ;
    pstMsg->ulReceiverPid = ulReceiverPid;
    pstMsg->usClientId    = usClientId;
    pstMsg->ucOpId        = ucOpId;
    pstMsg->ucIndex       = ucIndex;

    if (VOS_OK == VOS_SendMsg(WUEPS_PID_AT, pstMsg))
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
#else
    return VOS_ERR;
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


