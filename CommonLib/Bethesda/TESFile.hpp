#pragma once
#include "BSSimpleList.hpp"
#include "BSString.hpp"
#include "BSFile.hpp"
#include "NiTPointerMap.hpp"

class TESObjectCELL;

struct ChunkAndFormType {
	UInt32		chunkType;	// ie
	UInt32		formType;	// ie 0x2A
	const char* formName;	// ie 'NPC_'
};

#if RUNTIME
// addresses of static ModInfo members holding type info about currently loading form
static UInt32* s_ModInfo_CurrentChunkTypeCode = (UInt32*)0x011C54F4;
static UInt32* s_ModInfo_CurrentFormTypeEnum = (UInt32*)0x011C54F0;
// in last call (SetStaticFieldsAndGetFormTypeEnum) of first call (ModInfo__GetFormInfoTypeID) from _ModInfo_InitializeForm
//		s_ModInfo_CurrentChunkTypeCode is first cmp
//		s_ModInfo_CurrentChunkTypeEnum is next mov
static ChunkAndFormType* s_ModInfo_ChunkAndFormTypes = (ChunkAndFormType*)0x01187008;	// Array used in the loop in SetStaticFieldsAndGetFormTypeEnum, starts under dd offset aNone
#endif

struct ChunkHeader {
	UInt32	type : 4;	// i.e. 'XGRD', 'DATA'
	UInt16	size : 2;
};

// 0x42C
class TESFile 
{
public:
	TESFile();
	~TESFile();

	// 18 info about currently loading form
	struct FormInfo  // Record Header in FNVEdit
	{
		UInt32		recordType;			// 00 i.e. 'FACT', 'GMST'						Signature
		UInt32		dataSize;			// 04 looks like size of entire record			Data Size
		UInt32		formFlags;			// 08 copied to TESForm->flags					Record Flags
		UInt32		formID;				// 0C											FormID
		UInt32		unk10;				// 10											Version Control Info 1
		UInt16		formVersion;		// 14 always initialized to 0F on SaveForm.		Form Version
		UInt16		unk16;				// 16										   Version Control Info 2
	};

	// 18 info about current group of form
	struct GroupInfo  // Record Header in FNVEdit
	{
		UInt32		recordType;			// 00 'GRUP'									Signature
		UInt32		groupSize;			// 04 Size of entire record						Size
		UInt32		groupLabel;			// 08 copied to TESForm->flags					Label
		UInt32		groupType;			// 0C forms, dialog, cell...					Type
		UInt32		unk10;				// 10											Stamp
		UInt16		unk14;				// 14											Part of Unknown
		UInt16		unk16;				// 16										   Part of Unknown
	};

	struct FileHeader	// File header in FNVEdit Signature 'HEDR'
	{
		float version;			//	00
		UInt32	recordCount;	//	04
		UInt32	nextObectID;	//	08
	};

	struct	MasterSize	// Data member of the master list in WIN32_FIND_DATA format
	{
		UInt32	low;
		UInt32	high;
	};

	// Fallout 4 ones, will have to adapt
	enum CHUNK_ID
	{
		NO_CHUNK = 0x0,
		HEDR_ID = 0x52444548,
		MAST_ID = 0x5453414D,
		DELE_ID = 0x454C4544,
		SCRD_ID = 0x44524353,
		SCRS_ID = 0x53524353,
		NAME_ID = 0x454D414E,
		OBND_ID = 0x444E424F,
		MODL_ID = 0x4C444F4D,
		MODD_ID = 0x44444F4D,
		MOSD_ID = 0x44534F4D,
		MOD2_ID = 0x32444F4D,
		MOD3_ID = 0x33444F4D,
		MOD4_ID = 0x34444F4D,
		MOD5_ID = 0x35444F4D,
		MODT_ID = 0x54444F4D,
		MO2T_ID = 0x54324F4D,
		MO3T_ID = 0x54334F4D,
		MO4T_ID = 0x54344F4D,
		MO5T_ID = 0x54354F4D,
		MODS_ID = 0x53444F4D,
		MO2S_ID = 0x53324F4D,
		MO3S_ID = 0x53334F4D,
		MO4S_ID = 0x53344F4D,
		MO5S_ID = 0x53354F4D,
		MODC_ID = 0x43444F4D,
		MO2C_ID = 0x43324F4D,
		MO3C_ID = 0x43334F4D,
		MO4C_ID = 0x43344F4D,
		MO5C_ID = 0x43354F4D,
		MO2F_ID = 0x46324F4D,
		MO3F_ID = 0x46334F4D,
		MO4F_ID = 0x46344F4D,
		MO5F_ID = 0x46354F4D,
		CVPA_ID = 0x41505643,
		CDIX_ID = 0x58494443,
		FVPA_ID = 0x41505646,
		KFFZ_ID = 0x5A46464B,
		NIFZ_ID = 0x5A46494E,
		NIFT_ID = 0x5446494E,
		ANAM_ID = 0x4D414E41,
		BNAM_ID = 0x4D414E42,
		CNAM_ID = 0x4D414E43,
		DNAM_ID = 0x4D414E44,
		ENAM_ID = 0x4D414E45,
		FNAM_ID = 0x4D414E46,
		GNAM_ID = 0x4D414E47,
		HNAM_ID = 0x4D414E48,
		INAM_ID = 0x4D414E49,
		JNAM_ID = 0x4D414E4A,
		KNAM_ID = 0x4D414E4B,
		LNAM_ID = 0x4D414E4C,
		MNAM_ID = 0x4D414E4D,
		NNAM_ID = 0x4D414E4E,
		ONAM_ID = 0x4D414E4F,
		PNAM_ID = 0x4D414E50,
		CLNM_ID = 0x4D414E51,
		QNAM_ID = 0x4D414E51,
		RNAM_ID = 0x4D414E52,
		SNAM_ID = 0x4D414E53,
		TNAM_ID = 0x4D414E54,
		UNAM_ID = 0x4D414E55,
		VNAM_ID = 0x4D414E56,
		WNAM_ID = 0x4D414E57,
		XNAM_ID = 0x4D414E58,
		YNAM_ID = 0x4D414E59,
		ZNAM_ID = 0x4D414E5A,
		NAM1_ID = 0x314D414E,
		NAM2_ID = 0x324D414E,
		NAM3_ID = 0x334D414E,
		NAM4_ID = 0x344D414E,
		NAM5_ID = 0x354D414E,
		NAM6_ID = 0x364D414E,
		NAM7_ID = 0x374D414E,
		NAM8_ID = 0x384D414E,
		NAM9_ID = 0x394D414E,
		NAM0_ID = 0x304D414E,
		NAMA_ID = 0x414D414E,
		SPCT_ID = 0x54435053,
		COCT_ID = 0x54434F43,
		LLCT_ID = 0x54434C4C,
		PTRN_ID = 0x4E525450,
		ATTX_ID = 0x58545441,
		WTMX_ID = 0x584D5457,
		FTYP_ID = 0x50595446,
		PFRN_ID = 0x4E524650,
		WKMV_ID = 0x564D4B57,
		RNMV_ID = 0x564D4E52,
		SWMV_ID = 0x564D5753,
		FLMV_ID = 0x564D4C46,
		SNMV_ID = 0x564D4E53,
		SPMV_ID = 0x564D5053,
		QUAL_ID = 0x4C415551,
		DOFT_ID = 0x54464F44,
		SOFT_ID = 0x54464F53,
		DPLT_ID = 0x544C5044,
		SPOR_ID = 0x524F5053,
		OCOR_ID = 0x524F434F,
		GWOR_ID = 0x524F5747,
		ECOR_ID = 0x524F4345,
		CRIF_ID = 0x46495243,
		FCPL_ID = 0x4C504346,
		SHRT_ID = 0x54524853,
		RCLR_ID = 0x524C4352,
		RPLD_ID = 0x444C5052,
		RPLI_ID = 0x494C5052,
		RLDM_ID = 0x4D444C52,
		RDAT_ID = 0x54414452,
		RDOB_ID = 0x424F4452,
		RDOJ_ID = 0x4A4F4452,
		RDOT_ID = 0x544F4452,
		RDWT_ID = 0x54574452,
		RDMP_ID = 0x504D4452,
		RDLN_ID = 0x4E4C4452,
		RDGS_ID = 0x53474452,
		RDMD_ID = 0x444D4452,
		RDMO_ID = 0x4F4D4452,
		RDSD_ID = 0x44534452,
		RDSA_ID = 0x41534452,
		XIDX_ID = 0x58444958,
		XACT_ID = 0x54434158,
		XOWN_ID = 0x4E574F58,
		XGLB_ID = 0x424C4758,
		XRNK_ID = 0x4B4E5258,
		XHLT_ID = 0x544C4858,
		XHLP_ID = 0x504C4858,
		XUSE_ID = 0x45535558,
		XTIM_ID = 0x4D495458,
		XCNT_ID = 0x544E4358,
		XCHG_ID = 0x47484358,
		XSOL_ID = 0x4C4F5358,
		XSCL_ID = 0x4C435358,
		XLOC_ID = 0x434F4C58,
		XTEL_ID = 0x4C455458,
		XTNM_ID = 0x4D4E5458,
		XMRK_ID = 0x4B524D58,
		XEDL_ID = 0x4C444558,
		XPSL_ID = 0x4C535058,
		XSED_ID = 0x44455358,
		XPCI_ID = 0x49435058,
		XRGD_ID = 0x44475258,
		XRGB_ID = 0x42475258,
		XLOD_ID = 0x444F4C58,
		XESP_ID = 0x50534558,
		XRTM_ID = 0x4D545258,
		XLKR_ID = 0x524B4C58,
		XLKT_ID = 0x544B4C58,
		XATR_ID = 0x52544158,
		XATP_ID = 0x50544158,
		XACR_ID = 0x52434158,
		XAPD_ID = 0x44504158,
		XAPR_ID = 0x52504158,
		XMRC_ID = 0x43524D58,
		XHRS_ID = 0x53524858,
		XPSN_ID = 0x4E535058,
		XPSC_ID = 0x43535058,
		XSPC_ID = 0x43505358,
		XASP_ID = 0x50534158,
		XLCM_ID = 0x4D434C58,
		XDCR_ID = 0x52434458,
		XRDS_ID = 0x53445258,
		XRAD_ID = 0x44415258,
		XLMB_ID = 0x424D4C58,
		XMBR_ID = 0x52424D58,
		XMBO_ID = 0x4F424D58,
		XEMI_ID = 0x494D4558,
		XRDO_ID = 0x4F445258,
		XCGD_ID = 0x44474358,
		XCLC_ID = 0x434C4358,
		XCLL_ID = 0x4C4C4358,
		XCLW_ID = 0x574C4358,
		XCWT_ID = 0x54574358,
		XCLR_ID = 0x524C4358,
		XCLM_ID = 0x4D4C4358,
		XCMT_ID = 0x544D4358,
		XCMO_ID = 0x4F4D4358,
		XCET_ID = 0x54454358,
		XCCM_ID = 0x4D434358,
		XCIM_ID = 0x4D494358,
		XCAS_ID = 0x53414358,
		XPWR_ID = 0x52575058,
		XRFB_ID = 0x42465258,
		XWLT_ID = 0x544C5758,
		XLTW_ID = 0x57544C58,
		XNVP_ID = 0x50564E58,
		XNDP_ID = 0x50444E58,
		XLCR_ID = 0x504C4358,
		XPRM_ID = 0x4D525058,
		XPRD_ID = 0x44525058,
		XPPA_ID = 0x41505058,
		XOCP_ID = 0x50434F58,
		XORD_ID = 0x44524F58,
		XPOD_ID = 0x444F5058,
		XPTL_ID = 0x4C545058,
		XROO_ID = 0x4F4F5258,
		XRMR_ID = 0x524D5258,
		XLRM_ID = 0x4D524C58,
		XMBP_ID = 0x50424D58,
		XTRI_ID = 0x49525458,
		XAMC_ID = 0x434D4158,
		XAMT_ID = 0x544D4158,
		XEZN_ID = 0x4E5A4558,
		XLCN_ID = 0x4E434C58,
		XLRT_ID = 0x54524C58,
		XLRL_ID = 0x4C524C58,
		XLIB_ID = 0x42494C58,
		XIBS_ID = 0x53424958,
		XIS2_ID = 0x32534958,
		XLIG_ID = 0x47494C58,
		XHTW_ID = 0x57544858,
		XFVC_ID = 0x43564658,
		XALP_ID = 0x504C4158,
		XHOR_ID = 0x524F4858,
		XENC_ID = 0x434E4558,
		XILL_ID = 0x4C4C4958,
		XWNT_ID = 0x544E5758,
		XWEM_ID = 0x4D455758,
		XCVL_ID = 0x4C564358,
		XCVR_ID = 0x52564358,
		XWCD_ID = 0x55435758,
		XWCS_ID = 0x53435758,
		XWCN_ID = 0x4E435758,
		XCZR_ID = 0x525A4358,
		XCZC_ID = 0x435A4358,
		XCZA_ID = 0x415A4358,
		XBSD_ID = 0x44534258,
		XRFG_ID = 0x47465258,
		XLYR_ID = 0x52594C58,
		XMSP_ID = 0x50534D58,
		XCRI_ID = 0x49524358,
		XPRI_ID = 0x49525058,
		XPDD_ID = 0x44445058,
		XWSD_ID = 0x44535758,
		XILW_ID = 0x574C4958,
		XGDR_ID = 0x52444758,
		XWPG_ID = 0x47505758,
		XWPN_ID = 0x4E505758,
		XPLK_ID = 0x4B4C5058,
		XIVG_ID = 0x47564958,
		MTYP_ID = 0x5059544D,
		SPED_ID = 0x44455053,
		ACTN_ID = 0x4E544341,
		STPR_ID = 0x52505453,
		APPR_ID = 0x52505041,
		APGD_ID = 0x44475041,
		SOUL_ID = 0x4C554F53,
		SLCP_ID = 0x50434C53,
		CIDA_ID = 0x41445443,
		CIS1_ID = 0x31534943,
		CIS2_ID = 0x32534943,
		CITC_ID = 0x43544943,
		TRDT_ID = 0x54445254,
		TRDA_ID = 0x41445254,
		TCLT_ID = 0x544C4354,
		TCLF_ID = 0x464C4354,
		TWAT_ID = 0x54415754,
		TIFC_ID = 0x43464954,
		TSCE_ID = 0x45435354,
		ESCE_ID = 0x45435345,
		ESCS_ID = 0x53435345,
		ESBS_ID = 0x53425345,
		ESHS_ID = 0x53485345,
		ESAS_ID = 0x53415345,
		ATTN_ID = 0x4E545441,
		WIDX_ID = 0x58444957,
		MODQ_ID = 0x51444F4D,
		DATA_ID = 0x41544144,
		ATDM_ID = 0x4D445441,
		ACBS_ID = 0x53424341,
		ATTR_ID = 0x52545441,
		ATKD_ID = 0x444B5441,
		ATKE_ID = 0x454B5441,
		ATKR_ID = 0x524B5441,
		ATKW_ID = 0x574B5441,
		ATKS_ID = 0x534B5441,
		ATKT_ID = 0x544B5441,
		UNWP_ID = 0x50574E55,
		RBPC_ID = 0x43504252,
		DESC_ID = 0x43534544,
		INDX_ID = 0x58444E49,
		STRV_ID = 0x56525453,
		INTV_ID = 0x56544E49,
		FLTV_ID = 0x56544C46,
		ITMC_ID = 0x434D5449,
		ITMS_ID = 0x534D5449,
		ITME_ID = 0x454D5449,
		FLTR_ID = 0x52544C46,
		DODT_ID = 0x54444F44,
		NPAC_ID = 0x4341504E,
		ND3D_ID = 0x4433444E,
		ANIS_ID = 0x53494E41,
		CNTO_ID = 0x4F544E43,
		SPLO_ID = 0x4F4C5053,
		REPL_ID = 0x4C504552,
		MDOB_ID = 0x424F444D,
		BIPL_ID = 0x4C504942,
		LVLO_ID = 0x4F4C564C,
		LLOF_ID = 0x464F4C4C,
		LVLD_ID = 0x444C564C,
		LVLM_ID = 0x4D4C564C,
		LVLF_ID = 0x464C564C,
		LVLG_ID = 0x474C564C,
		LVSG_ID = 0x4753564C,
		LLKC_ID = 0x434B4C4C,
		EFID_ID = 0x44494645,
		EFIT_ID = 0x54494645,
		AVIT_ID = 0x54495641,
		SCIT_ID = 0x54494353,
		SPIT_ID = 0x54495053,
		ENIT_ID = 0x54494E45,
		EITM_ID = 0x4D544945,
		EAMT_ID = 0x544D4145,
		PKID_ID = 0x44494B50,
		PKDT_ID = 0x54444B50,
		PLDT_ID = 0x54444C50,
		PLD2_ID = 0x32444C50,
		PSDT_ID = 0x54445350,
		PTDT_ID = 0x54445450,
		PTD2_ID = 0x32445450,
		PTDA_ID = 0x41445450,
		PT2A_ID = 0x41325450,
		PKDD_ID = 0x44444B50,
		PKED_ID = 0x44454B50,
		PKND_ID = 0x444E4B50,
		PKFD_ID = 0x44464B50,
		PKW3_ID = 0x33574B50,
		PUID_ID = 0x44495550,
		PKPT_ID = 0x54504B50,
		PKES_ID = 0x32454B50,
		PKCU_ID = 0x55434B50,
		PKC2_ID = 0x32434B50,
		PKAM_ID = 0x4D414B50,
		POBA_ID = 0x41424F50,
		POEA_ID = 0x41454F50,
		POCA_ID = 0x41434F50,
		PRCB_ID = 0x42435250,
		PFOR_ID = 0x524F4650,
		PFO2_ID = 0x324F4650,
		PDTO_ID = 0x4F544450,
		BIDX_ID = 0x58444942,
		BPTM_ID = 0x4D545042,
		BPTF_ID = 0x46545042,
		BMDT_ID = 0x54444D42,
		BMCT_ID = 0x54434D42,
		BODT_ID = 0x54444F42,
		BOD2_ID = 0x32444F42,
		PFIG_ID = 0x47494650,
		PFPC_ID = 0x43504650,
		KSIZ_ID = 0x5A49534B,
		KWDA_ID = 0x4144574B,
		MVRF_ID = 0x4652564D,
		AGTY_ID = 0x59544741,
		AGFD_ID = 0x44464741,
		AGSC_ID = 0x43534741,
		AGSX_ID = 0x58534741,
		ICON_ID = 0x4E4F4349,
		ICO2_ID = 0x324F4349,
		SCRI_ID = 0x49524353,
		SCHR_ID = 0x52484353,
		SCVR_ID = 0x52564353,
		SCTX_ID = 0x58544353,
		SCDA_ID = 0x41444353,
		SCRO_ID = 0x4F524353,
		SCRV_ID = 0x56524353,
		SLCS_ID = 0x53434C53,
		SLSD_ID = 0x44534C53,
		SLLD_ID = 0x444C4C53,
		SLFD_ID = 0x44464C53,
		SLUD_ID = 0x44554C53,
		VHGT_ID = 0x54474856,
		VNML_ID = 0x4C4D4E56,
		VCLR_ID = 0x524C4356,
		BTXT_ID = 0x54585442,
		ATXT_ID = 0x54585441,
		VTXT_ID = 0x54585456,
		MPCD_ID = 0x4443504D,
		AIDT_ID = 0x54444941,
		PGRP_ID = 0x50524750,
		PGRC_ID = 0x52524750,
		PGRI_ID = 0x49524750,
		PGRL_ID = 0x4C524750,
		PGAG_ID = 0x47414750,
		LVCR_ID = 0x5243564C,
		GMDT_ID = 0x54444D47,
		QSTI_ID = 0x49545351,
		QSDT_ID = 0x54445351,
		QSTA_ID = 0x41545351,
		QOBJ_ID = 0x4A424F51,
		QTGL_ID = 0x4C475451,
		TPIC_ID = 0x43495054,
		QSTR_ID = 0x52545351,
		WHGT_ID = 0x54474857,
		EDID_ID = 0x44494445,
		FULL_ID = 0x4C4C5546,
		MHDT_ID = 0x5444484D,
		TVDT_ID = 0x54445654,
		LODH_ID = 0x48444F4C,
		LODV_ID = 0x56444F4C,
		LODS_ID = 0x53444F4C,
		LODT_ID = 0x54444F4C,
		LODC_ID = 0x43444F4C,
		LODN_ID = 0x4E444F4C,
		FGGS_ID = 0x53474746,
		FGGA_ID = 0x41474746,
		FGTS_ID = 0x53544746,
		HCLR_ID = 0x524C4348,
		HCLF_ID = 0x464C4348,
		BCLF_ID = 0x464C4342,
		HLTX_ID = 0x58544C48,
		OFST_ID = 0x5453464F,
		CLSZ_ID = 0x5A534C43,
		SNDD_ID = 0x44444E53,
		SDSC_ID = 0x43534453,
		SNDX_ID = 0x58444E53,
		CSCR_ID = 0x52435343,
		CSDT_ID = 0x54445343,
		CSDI_ID = 0x49445343,
		CSDC_ID = 0x43445343,
		CS2H_ID = 0x48325343,
		CS2D_ID = 0x44325343,
		CS2K_ID = 0x4B325343,
		CS2E_ID = 0x45325343,
		CS2F_ID = 0x46325343,
		CSGD_ID = 0x44475343,
		CSME_ID = 0x454D5343,
		CSRA_ID = 0x41525343,
		CSLR_ID = 0x524C5343,
		CSCV_ID = 0x56435343,
		CSFL_ID = 0x4C465343,
		WLST_ID = 0x54534C57,
		LVSR_ID = 0x5053564C,
		EFSD_ID = 0x44534645,
		IDLC_ID = 0x434C4449,
		IDLA_ID = 0x414C4449,
		IDLF_ID = 0x464C4449,
		IDLT_ID = 0x544C4449,
		IDLB_ID = 0x424C4449,
		IDLN_ID = 0x4E4C4449,
		BIDS_ID = 0x53444942,
		BAMT_ID = 0x544D4142,
		PROD_ID = 0x444F5250,
		NVNM_ID = 0x4D4E564E,
		NVER_ID = 0x5245564E,
		DEST_ID = 0x54534544,
		DSTD_ID = 0x44545344,
		DSTA_ID = 0x41545344,
		DSTF_ID = 0x46545344,
		DMDL_ID = 0x4C444D44,
		DMDT_ID = 0x54444D44,
		DMDS_ID = 0x53444D44,
		DMDC_ID = 0x43444D44,
		TX00_ID = 0x30305854,
		TX01_ID = 0x31305854,
		TX02_ID = 0x32305854,
		TX03_ID = 0x33305854,
		TX04_ID = 0x34305854,
		TX05_ID = 0x35305854,
		TX06_ID = 0x36305854,
		TX07_ID = 0x37305854,
		TX08_ID = 0x38305854,
		TX09_ID = 0x39305854,
		_0TX_ID = 0x58543030,
		DMTL_ID = 0x4C544D44,
		NVMI_ID = 0x494D564E,
		NVCI_ID = 0x4943564E,
		NVPP_ID = 0x5050564E,
		BPTN_ID = 0x4E545042,
		BPNN_ID = 0x4E4E5042,
		BPNT_ID = 0x544E5042,
		BPNI_ID = 0x494E5042,
		BPND_ID = 0x444E5042,
		ETYP_ID = 0x50595445,
		WBDT_ID = 0x54444257,
		FNPR_ID = 0x52504E46,
		FNMK_ID = 0x4B4D4E46,
		PRKE_ID = 0x454B5250,
		PRKF_ID = 0x464B5250,
		PRKC_ID = 0x434B5250,
		EPFT_ID = 0x54465045,
		EPFB_ID = 0x42465045,
		EPFD_ID = 0x44465045,
		EPF2_ID = 0x32465045,
		EPF3_ID = 0x33465045,
		PRKL_ID = 0x4C4B5250,
		PRKR_ID = 0x524B5250,
		PRKZ_ID = 0x5A4B5250,
		ITID_ID = 0x44495449,
		ITXT_ID = 0x54585449,
		ISCR_ID = 0x52435349,
		ISIZ_ID = 0x5A495349,
		BSIZ_ID = 0x5A495342,
		CRDT_ID = 0x54445243,
		COED_ID = 0x44454F43,
		SIAD_ID = 0x44414900,
		EIAD_ID = 0x444149FF,
		IMSP_ID = 0x50534D49,
		GDRD_ID = 0x52444757,
		VTCK_ID = 0x4B435456,
		TPLT_ID = 0x544C5054,
		TPTA_ID = 0x41545054,
		LTPT_ID = 0x5450544C,
		LTPC_ID = 0x4350544C,
		MICO_ID = 0x4F43494D,
		MIC2_ID = 0x3243494D,
		NEXT_ID = 0x5458454E,
		LTMP_ID = 0x504D544C,
		RAFD_ID = 0x44464152,
		RAFB_ID = 0x42464152,
		RAFS_ID = 0x53464152,
		RAFL_ID = 0x4C464152,
		RAFN_ID = 0x4E464152,
		RALH_ID = 0x484C4152,
		RALN_ID = 0x4E4C4152,
		RAGA_ID = 0x41474152,
		RAGN_ID = 0x4E474152,
		RAPS_ID = 0x53504152,
		WCTR_ID = 0x52544357,
		LCPR_ID = 0x5250434C,
		ACPR_ID = 0x52504341,
		RCPR_ID = 0x52504352,
		LCUN_ID = 0x4E55434C,
		ACUN_ID = 0x4E554341,
		RCUN_ID = 0x4E554352,
		LCSR_ID = 0x5253434C,
		ACSR_ID = 0x52534341,
		RCSR_ID = 0x52534352,
		LCEC_ID = 0x4345434C,
		ACEC_ID = 0x43454341,
		RCEC_ID = 0x43454352,
		LCID_ID = 0x4449434C,
		ACID_ID = 0x44494341,
		LCEP_ID = 0x5045434C,
		ACEP_ID = 0x50454341,
		ALID_ID = 0x44494C41,
		ALCC_ID = 0x43434C41,
		ALFA_ID = 0x41464C41,
		ALFE_ID = 0x45464C41,
		ALNA_ID = 0x414E4C41,
		ALNT_ID = 0x544E4C41,
		ALFD_ID = 0x44464C41,
		ALFI_ID = 0x49464C41,
		ALEQ_ID = 0x51454C41,
		ALEA_ID = 0x41454C41,
		ALLS_ID = 0x534C4C41,
		ALFL_ID = 0x4C464C41,
		ALST_ID = 0x54534C41,
		ALFR_ID = 0x52464C41,
		ALRT_ID = 0x54524C41,
		ALCO_ID = 0x4F434C41,
		ALCA_ID = 0x41434C41,
		ALCL_ID = 0x4C434C41,
		ALUA_ID = 0x41554C41,
		ALSC_ID = 0x43534C41,
		ALDN_ID = 0x4E444C41,
		ALSP_ID = 0x50534C41,
		ALFC_ID = 0x43464C41,
		ALPC_ID = 0x43504C41,
		ALED_ID = 0x44454C41,
		ALLA_ID = 0x414C4C41,
		ALDI_ID = 0x49444C41,
		ALFV_ID = 0x56464C41,
		ALCS_ID = 0x53434C41,
		ALMI_ID = 0x494D4C41,
		MLDS_ID = 0x53444C4D,
		MLDI_ID = 0x49444C4D,
		MLDE_ID = 0x45444C4D,
		MPRT_ID = 0x5452504D,
		FPRT_ID = 0x54525046,
		MCHT_ID = 0x5448434D,
		FCHT_ID = 0x54484346,
		AVSK_ID = 0x4B535641,
		VMAD_ID = 0x44414D56,
		MTNM_ID = 0x4D4E544D,
		MSPD_ID = 0x4450534D,
		JAIL_ID = 0x4C49414A,
		WAIT_ID = 0x54494157,
		STOL_ID = 0x4C4F5453,
		PLCN_ID = 0x4E434C50,
		CRGR_ID = 0x52475243,
		CRVA_ID = 0x41565243,
		XXXX_ID = 0x58585858,
		HTID_ID = 0x44495448,
		DMAX_ID = 0x58414D44,
		DMIN_ID = 0x4E494D44,
		DEMO_ID = 0x4F4D4544,
		DEVA_ID = 0x41564544,
		PHKV_ID = 0x564B4850,
		PHWT_ID = 0x54574850,
		PHTN_ID = 0x4E544850,
		VEND_ID = 0x444E4556,
		VENV_ID = 0x564E4556,
		VENC_ID = 0x434E4556,
		PLVD_ID = 0x44564C50,
		JOUT_ID = 0x54554F4A,
		DALC_ID = 0x434C4144,
		HEAD_ID = 0x44414548,
		TINT_ID = 0x544E4954,
		TINC_ID = 0x434E4954,
		TIND_ID = 0x444E4954,
		TINV_ID = 0x564E4954,
		TINI_ID = 0x494E4954,
		TINL_ID = 0x4C4E4954,
		TINP_ID = 0x504E4954,
		TIAS_ID = 0x53414954,
		TIRS_ID = 0x53524954,
		TTGP_ID = 0x50475454,
		TTGE_ID = 0x45475454,
		TTEN_ID = 0x50475454,
		TTET_ID = 0x54455454,
		TTEC_ID = 0x43455454,
		TTED_ID = 0x44455454,
		TTEF_ID = 0x46455454,
		TETI_ID = 0x49544554,
		TEND_ID = 0x444E4554,
		TTEB_ID = 0x42455454,
		MPAI_ID = 0x4941504D,
		MPAV_ID = 0x5641504D,
		RPRM_ID = 0x4D525052,
		RPRF_ID = 0x46525052,
		AHCM_ID = 0x4D434841,
		AHCF_ID = 0x46434841,
		FTST_ID = 0x54535446,
		FTSM_ID = 0x4D535446,
		FTSF_ID = 0x46535446,
		DFTM_ID = 0x4D544644,
		DFTF_ID = 0x46544644,
		MSDK_ID = 0x4B44534D,
		MSDV_ID = 0x5644534D,
		MSID_ID = 0x4449534D,
		MSM0_ID = 0x304D534D,
		MSM1_ID = 0x314D534D,
		MPGN_ID = 0x4E47504D,
		MPGP_ID = 0x5047504D,
		MPPC_ID = 0x4350504D,
		MPPN_ID = 0x4E50504D,
		MPPM_ID = 0x4D50504D,
		MPPT_ID = 0x5450504D,
		MPPI_ID = 0x4950504D,
		MPPV_ID = 0x5650504D,
		MPPS_ID = 0x5350504D,
		MPPK_ID = 0x4B50504D,
		MPPF_ID = 0x4650504D,
		MPGS_ID = 0x5347504D,
		MLSI_ID = 0x49534C4D,
		MRSV_ID = 0x5653524D,
		MWGT_ID = 0x5447574D,
		MXWT_ID = 0x5457584D,
		FMRI_ID = 0x49524D46,
		FMRN_ID = 0x4E524D46,
		FMRS_ID = 0x53524D46,
		FMRC_ID = 0x43524D46,
		FMRB_ID = 0x42524D46,
		FMRD_ID = 0x44524D46,
		FMIN_ID = 0x4E494D46,
		WMAP_ID = 0x50414D57,
		PTOP_ID = 0x504F5450,
		NTOP_ID = 0x504F544E,
		NETO_ID = 0x4F54454E,
		QTOP_ID = 0x504F5451,
		DTID_ID = 0x44495444,
		DTGT_ID = 0x54475444,
		NPOT_ID = 0x544F504E,
		NNGT_ID = 0x54474E4E,
		NNUT_ID = 0x54554E4E,
		NQUT_ID = 0x5455514E,
		NPOS_ID = 0x534F504E,
		NNGS_ID = 0x53474E4E,
		NNUS_ID = 0x53554E4E,
		NQUS_ID = 0x5355514E,
		STSC_ID = 0x43535453,
		INTT_ID = 0x54544E49,
		ROSE_ID = 0x45534F52,
		GREE_ID = 0x45455247,
		PERI_ID = 0x4B524550,
		ACTV_ID = 0x56544341,
		CRIS_ID = 0x53495243,
		TIQS_ID = 0x53514954,
		SCQS_ID = 0x53514353,
		IOVR_ID = 0x52564F49,
		INCC_ID = 0x43434E49,
		WAMD_ID = 0x444D4157,
		SRAC_ID = 0x43415253,
		SADD_ID = 0x44444153,
		SAKD_ID = 0x444B4153,
		STKD_ID = 0x444B5453,
		SGNM_ID = 0x4D4E4753,
		SAPT_ID = 0x54504153,
		SRAF_ID = 0x46415253,
		OBTE_ID = 0x4554424F,
		OBML_ID = 0x4C4D424F,
		OBLT_ID = 0x544C424F,
		OBTM_ID = 0x4D54424F,
		OBTS_ID = 0x5354424F,
		OBTF_ID = 0x4654424F,
		OBTL_ID = 0x4C54424F,
		OBFL_ID = 0x4C46424F,
		STOP_ID = 0x504F5453,
		WZMD_ID = 0x444D5A57,
		INRD_ID = 0x44524E49,
		DAMA_ID = 0x414D4144,
		DAMC_ID = 0x434D4144,
		PRPS_ID = 0x53505250,
		RADR_ID = 0x52444152,
		REPT_ID = 0x54504552,
		AVMG_ID = 0x474D5641,
		AVFL_ID = 0x4C465641,
		AORD_ID = 0x44524F41,
		AOR2_ID = 0x32524F41,
		NTRM_ID = 0x4D52544E,
		CUSD_ID = 0x44535543,
		SSPN_ID = 0x4E505353,
		STCP_ID = 0x50435453,
		VISI_ID = 0x49534956,
		PCMB_ID = 0x424D4350,
		FIMD_ID = 0x444D4946,
		RVIS_ID = 0x53495652,
		WLEV_ID = 0x56454C57,
		BSMP_ID = 0x504D5342,
		BSMB_ID = 0x424D5342,
		BSMS_ID = 0x534D5342,
		BMMP_ID = 0x504D4D42,
		MASE_ID = 0x4553414D,
		LFSP_ID = 0x5053464C,
		LFSD_ID = 0x4453464C,
	};

	enum Flags {
		MASTER			= 0x1,
		ALTERED			= 0x2,
		CHECKED			= 0x4,
		ACTIVE			= 0x8,
		OPTIMIZED		= 0x10,
		TEMP_ID_OWNER	= 0x20,
		ENDIAN_SWAPPED	= 0x40,
		DELOCALIZED		= 0x80,
	};

	UInt32								m_lastError;
	TESFile*							pThreadSafeParent;
	NiTPointerMap<UInt32, TESFile*>*	kThreadSafeFileMap;
	BSFile*								m_pLockedFile;
	BSFile*								m_pFile;
	struct TESBitArrayFile*				pFormUserDataBitArray;
	struct TESBitArrayFile*				pFormVersionBitArray;
	struct TESBitArrayFile*				pFormIDBitArray;
	char								m_Filename[260];
	char								m_Path[260];
	char*								m_pBuffer;
	UInt32								m_uiBufferAllocSize;
	UInt32								m_firstCellOffset;
	UInt32								m_currCellOffset;
	TESObjectCELL*						m_pCurrCell;
	UInt32								m_currRefOffset;
	FormInfo							m_currentForm;
	CHUNK_ID							m_currentChunkID;
	UInt32								m_actualChunkSize;
	UInt32								m_fileSize;
	UInt32								m_fileOffset;
	UInt32								m_formOffset;
	UInt32								m_chunkOffset;
	FormInfo							m_saveForm;
	UInt32								m_saveFormOffset;
	UInt32								m_saveChunkOffset;
	BSSimpleList<UInt32>				m_groupList;
	bool								bHasGroups;
	bool								bIsBigEndian;
	bool								bForceCloseFiles;
	WIN32_FIND_DATA						fileData;
	FileHeader							header;
	UInt32								m_Flags;
	BSSimpleList<char*>					listMasters;
	BSSimpleList<UInt32>				refModData;
	UInt32								iMasterCount;
	UInt32**							m_pMasterPtrs;
	UInt32								unk404;
	UInt32								unk408;
	UInt8								modIndex;
	BSStringT<char>						author;
	BSStringT<char>						description;
	void*								pDecompressedFormBuffer;
	UInt32								iDecompressedFormBufferSize;
	bool								maybeShouldBeReloaded;

	// In Editor: 430 = ONAM array and 434 ONAM array count. Allocated at 0438

	bool IsLoaded() const { return true; }
	const char* GetName() const { return m_Filename; }
};
static_assert(sizeof(WIN32_FIND_DATA) == 0x140);
static_assert(sizeof(TESFile) == 0x42C);