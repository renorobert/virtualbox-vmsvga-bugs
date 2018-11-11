#include <stdint.h>

#pragma pack(4)
typedef struct VBoxGuestPortInfo
{
    uint32_t portAddress;
    void *pVMMDevMemory;
} VBoxGuestPortInfo;

/**
 * VMMDev request types.
 * @note when updating this, adjust vmmdevGetRequestSize() as well
 */
typedef enum
{
    VMMDevReq_InvalidRequest             =  0,
    VMMDevReq_GetMouseStatus             =  1,
    VMMDevReq_SetMouseStatus             =  2,
    VMMDevReq_SetPointerShape            =  3,
    VMMDevReq_GetHostVersion             =  4,
    VMMDevReq_Idle                       =  5,
    VMMDevReq_GetHostTime                = 10,
    VMMDevReq_GetHypervisorInfo          = 20,
    VMMDevReq_SetHypervisorInfo          = 21,
    VMMDevReq_RegisterPatchMemory        = 22, /* since version 3.0.6 */
    VMMDevReq_DeregisterPatchMemory      = 23, /* since version 3.0.6 */
    VMMDevReq_SetPowerStatus             = 30,
    VMMDevReq_AcknowledgeEvents          = 41,
    VMMDevReq_CtlGuestFilterMask         = 42,
    VMMDevReq_ReportGuestInfo            = 50,
    VMMDevReq_ReportGuestInfo2           = 58, /* since version 3.2.0 */
    VMMDevReq_ReportGuestStatus          = 59, /* since version 3.2.8 */
    VMMDevReq_ReportGuestUserState       = 74, /* since version 4.3 */
    /**
     * Retrieve a display resize request sent by the host using
     * @a IDisplay:setVideoModeHint.  Deprecated.
     *
     * Similar to @a VMMDevReq_GetDisplayChangeRequest2, except that it only
     * considers host requests sent for the first virtual display.  This guest
     * request should not be used in new guest code, and the results are
     * undefined if a guest mixes calls to this and
     * @a VMMDevReq_GetDisplayChangeRequest2.
     */
    VMMDevReq_GetDisplayChangeRequest    = 51,
    VMMDevReq_VideoModeSupported         = 52,
    VMMDevReq_GetHeightReduction         = 53,
    /**
     * Retrieve a display resize request sent by the host using
     * @a IDisplay:setVideoModeHint.
     *
     * Queries a display resize request sent from the host.  If the
     * @a eventAck member is sent to true and there is an unqueried
     * request available for one of the virtual display then that request will
     * be returned.  If several displays have unqueried requests the lowest
     * numbered display will be chosen first.  Only the most recent unseen
     * request for each display is remembered.
     * If @a eventAck is set to false, the last host request queried with
     * @a eventAck set is resent, or failing that the most recent received from
     * the host.  If no host request was ever received then all zeros are
     * returned.
     */
    VMMDevReq_GetDisplayChangeRequest2   = 54,
    VMMDevReq_ReportGuestCapabilities    = 55,
    VMMDevReq_SetGuestCapabilities       = 56,
    VMMDevReq_VideoModeSupported2        = 57, /* since version 3.2.0 */
    VMMDevReq_GetDisplayChangeRequestEx  = 80, /* since version 4.2.4 */
#ifdef VBOX_WITH_HGCM
    VMMDevReq_HGCMConnect                = 60,
    VMMDevReq_HGCMDisconnect             = 61,
#ifdef VBOX_WITH_64_BITS_GUESTS
    VMMDevReq_HGCMCall32                 = 62,
    VMMDevReq_HGCMCall64                 = 63,
#else
    VMMDevReq_HGCMCall                   = 62,
#endif /* VBOX_WITH_64_BITS_GUESTS */
    VMMDevReq_HGCMCancel                 = 64,
    VMMDevReq_HGCMCancel2                = 65,
#endif
    VMMDevReq_VideoAccelEnable           = 70,
    VMMDevReq_VideoAccelFlush            = 71,
    VMMDevReq_VideoSetVisibleRegion      = 72,
    VMMDevReq_GetSeamlessChangeRequest   = 73,
    VMMDevReq_QueryCredentials           = 100,
    VMMDevReq_ReportCredentialsJudgement = 101,
    VMMDevReq_ReportGuestStats           = 110,
    VMMDevReq_GetMemBalloonChangeRequest = 111,
    VMMDevReq_GetStatisticsChangeRequest = 112,
    VMMDevReq_ChangeMemBalloon           = 113,
    VMMDevReq_GetVRDPChangeRequest       = 150,
    VMMDevReq_LogString                  = 200,
    VMMDevReq_GetCpuHotPlugRequest       = 210,
    VMMDevReq_SetCpuHotPlugStatus        = 211,
    VMMDevReq_RegisterSharedModule       = 212,
    VMMDevReq_UnregisterSharedModule     = 213,
    VMMDevReq_CheckSharedModules         = 214,
    VMMDevReq_GetPageSharingStatus       = 215,
    VMMDevReq_DebugIsPageShared          = 216,
    VMMDevReq_GetSessionId               = 217, /* since version 3.2.8 */
    VMMDevReq_WriteCoreDump              = 218,
    VMMDevReq_GuestHeartbeat             = 219,
    VMMDevReq_HeartbeatConfigure         = 220,
    VMMDevReq_SizeHack                   = 0x7fffffff
} VMMDevRequestType;

/** Version of VMMDevRequestHeader structure. */
#define VMMDEV_REQUEST_HEADER_VERSION (0x10001)


/**
 * Generic VMMDev request header.
 */
typedef struct
{
    /** IN: Size of the structure in bytes (including body). */
    uint32_t size;
    /** IN: Version of the structure.  */
    uint32_t version;
    /** IN: Type of the request. */
    VMMDevRequestType requestType;
    /** OUT: Return code. */
    int32_t  rc;
    /** Reserved field no.1. MBZ. */
    uint32_t reserved1;
    /** Reserved field no.2. MBZ. */
    uint32_t reserved2;
} __attribute__((packed)) VMMDevRequestHeader;

/**
 * HGCM service location types.
 * @ingroup grp_vmmdev_req
 */
typedef enum
{
    VMMDevHGCMLoc_Invalid    = 0,
    VMMDevHGCMLoc_LocalHost  = 1,
    VMMDevHGCMLoc_LocalHost_Existing = 2,
    VMMDevHGCMLoc_SizeHack   = 0x7fffffff
} HGCMServiceLocationType;

/**
 * HGCM host service location.
 * @ingroup grp_vmmdev_req
 */
typedef struct
{
    char achName[128]; /**< This is really szName. */
} __attribute__((packed)) HGCMServiceLocationHost;

/**
 * HGCM service location.
 * @ingroup grp_vmmdev_req
 */
typedef struct HGCMSERVICELOCATION
{
    /** Type of the location. */
    HGCMServiceLocationType type;

    union
    {
        HGCMServiceLocationHost host;
    } u;
} __attribute__((packed)) HGCMServiceLocation;


/**
 * HGCM request header.
 */
typedef struct VMMDevHGCMRequestHeader
{
    /** Request header. */
    VMMDevRequestHeader header;

    /** HGCM flags. */
    uint32_t fu32Flags;

    /** Result code. */
    int32_t result;
} __attribute__((packed)) VMMDevHGCMRequestHeader;

/**
 * HGCM connect request structure.
 *
 * Used by VMMDevReq_HGCMConnect.
 */
typedef struct
{
    /** HGCM request header. */
    VMMDevHGCMRequestHeader header;

    /** IN: Description of service to connect to. */
    HGCMServiceLocation loc;

    /** OUT: Client identifier assigned by local instance of HGCM. */
    uint32_t u32ClientID;
} __attribute__((packed)) VMMDevHGCMConnect;

/**
 * HGCM disconnect request structure.
 *
 * Used by VMMDevReq_HGCMDisconnect.
 */
typedef struct
{
    /** HGCM request header. */
    VMMDevHGCMRequestHeader header;

    /** IN: Client identifier. */
    uint32_t u32ClientID;
} __attribute__((packed)) VMMDevHGCMDisconnect;

uint64_t gva_to_gpa(uint64_t);
