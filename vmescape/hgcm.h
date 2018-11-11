#include <stdint.h>

/**
 * AVL key type
 */
typedef unsigned long   AVLULKEY;

/**
 * AVL Core node.
 */
typedef struct _AVLULNodeCore
{
    AVLULKEY                Key;        /** Key value. */
    struct _AVLULNodeCore  *pLeft;      /** Pointer to left leaf node. */
    struct _AVLULNodeCore  *pRight;     /** Pointer to right leaf node. */
    unsigned char           uchHeight;  /** Height of this tree: max(height(left), height(right)) + 1 */
} AVLULNODECORE, *PAVLULNODECORE, **PPAVLULNODECORE;


typedef struct _ObjectAVLCore
{
    AVLULNODECORE AvlCore;
    void *pSelf;                        // type HGCMObject
} ObjectAVLCore;

typedef enum
{
    HGCMOBJ_CLIENT,
    HGCMOBJ_THREAD,
    HGCMOBJ_MSG,
    HGCMOBJ_SizeHack   = 0x7fffffff
} HGCMOBJ_TYPE;

struct HGCMClient {
        void *vptr_HGCMObject;
        uint32_t m_cRefs;
        uint32_t m_enmObjType;          // HGCMOBJ_TYPE enum
        ObjectAVLCore m_core;
        void *pService;                 // type HGCMService
        void *pvData;
        uint64_t padding;
} HGCMClient;

