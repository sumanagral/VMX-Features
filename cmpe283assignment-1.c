/*
 *  cmpe283-1.c - Kernel module for CMPE283 assignment 1
 */
#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <asm/msr.h>

#define MAX_MSG 80

/*
 * Model specific registers (MSRs) by the module.
 * See SDM volume 4, section 2.1
 */
#define IA32_VMX_PINBASED_CTLS 0x481
#define IA32_VMX_PROCBASED_CTLS 0x482
#define IA32_VMX_EXIT_CTLS 0x483
#define IA32_VMX_ENTRY_CTLS 0x484
#define IA32_VMX_PROCBASED_CTLS2 0x48B
#define IA32_VMX_PROCBASED_CTLS3 0x492

/*
 * struct caapability_info
 *
 * Represents a single capability (bit number and description).
 * Used by report_capability to output VMX capabilities.
 */
struct capability_info
{
    uint8_t bit;
    const char *name;
};

/*
 * Pinbased capabilities
 * See SDM volume 3, section 24.6.1
 */
struct capability_info pinbased[5] =
    {
        {0, "EXTERNAL INTERRUPT EXITING"},
        {3, "NMI EXITING"},
        {5, "VIRTUAL NMIS"},
        {6, "ACTIVATE VMX PREEMPTION TIMER"},
        {7, "PROCESS POSTED INTERRUPTS"}};

/*
 * Procbased capabilities
 */
struct capability_info procbased[22] =
    {
        {2, "INTERRUPT WINDOW EXITING"},
        {3, "USE TSC OFFSETTING"},
        {7, "HLT EXITING"},
        {9, "INVLPG EXITING"},
        {10, "MWAIT EXITING"},
        {11, "RDPMC EXITING"},
        {12, "RDTSC EXITING"},
        {15, "CR3 LOAD EXITING"},
        {16, "CR3 STORE EXITING"},
        {17, "ACTIVATE TERTIARY CONTROLS"},
        {19, "CR8 LOAD EXITING"},
        {20, "CR8 STORE EXITING"},
        {21, "USE TPR SHADOW"},
        {22, "NMI WINDOW EXITING"},
        {23, "MOV-DR EXITING"},
        {24, "UNCONDITIONAL I/O EXITING"},
        {25, "USE I/O BITMAPS"},
        {27, "MONITOR TRAP FLAG"},
        {28, "USE MSR BITMAPS"},
        {29, "MONITOR EXITING"},
        {30, "PAUSE EXITING"},
        {31, "ACTIVATE SECONDARY CONTROLS"}};

/*
 * Exit controls
 */
struct capability_info exit_controls[16] =
    {
        {2, "SAVE DEBUG CONTROLS"},
        {9, "HOST ADDRESS SPACE SIZE"},
        {12, "LOAD IA32_PERF_GLOBAL_CTRL"},
        {15, "ACKNOWLEDGE INTERRUPT ON EXIT"},
        {18, "SAVE IA32_PAT"},
        {19, "LOAD IA32_PAT"},
        {20, "SAVE IA32_EFER"},
        {21, "LOAD IA32_EFER"},
        {22, "SAVE VMXPREEMPTION TIMER VALUE"},
        {23, "CLEAR IA32_BNDCFGS"},
        {24, "CONCEAL VMX FROM PT"},
        {25, "CLEAR IA32_RTIT_CTL"},
        {26, "CLEAR IA32_LBR_CTL"},
        {28, "LOAD CET STATE"},
        {29, "LOAD PKRS"},
        {31, "ACTIVATE SECONDARY CONTROLS"},
};

/*
 * Entry controls
 */
struct capability_info entry_controls[13] =
    {
        {2, "LOAD DEBUG CONTROLS"},
        {9, "IA-32E MODE GUEST"},
        {10, "ENTRY TO SMM"},
        {11, "DEACTIVATE DUAL MONITOR TREATMENT"},
        {13, "LOAD IA32_PERF_GLOBAL_CTRL"},
        {14, "LOAD IA32_PAT"},
        {15, "LOAD IA32_EFER"},
        {16, "LOAD IA32_BNDCFGS"},
        {17, "CONCEAL VMX FROM PT"},
        {18, "LOAD IA32_RTIT_CTL"},
        {20, "LOAD CET STATE"},
        {21, "LOAD GUEST IA32_LBR_CTL"},
        {22, "LOAD PKRS"}};

/*
 * Secondary Procbased controls
 */
struct capability_info secondary_procbased[28] =
    {
        {0, "VIRTUALIZE APIC ACCESSES"},
        {1, "ENABLE EPT"},
        {2, "DESCRIPTOR TABLE EXITING"},
        {3, "ENABLE RDTSCP"},
        {4, "VIRTUALIZE X2APIC MODE"},
        {5, "ENABLE VPID"},
        {6, "WBINVD EXITING"},
        {7, "UNRESTRICTED GUEST"},
        {8, "APIC-REGISTER VIRTUALIZATION"},
        {9, "VIRTUAL-INTERRUPT DELIVERY"},
        {10, "PAUSE-LOOP EXITING"},
        {11, "RDRAND EXITING"},
        {12, "ENABLE INVPCID"},
        {13, "ENABLE VM FUNCTIONS"},
        {14, "VMCS SHADOWING"},
        {15, "ENABLE ENCLS EXITING"},
        {16, "RDSEED EXITING"},
        {17, "ENABLE PML"},
        {18, "EPT-VIOLATION #VE"},
        {19, "CONCEAL VMX FROM PT"},
        {20, "ENABLE XSAVES/XRSTORS"},
        {22, "MODE-BASED EXECUTE CONTROL FOR EPT"},
        {23, "SUB-PAGE WRITE PERMISSIONS FOR EPT"},
        {24, "INTEL PT USES GUEST PHYSICAL ADDRESSES"},
        {25, "USE TSC SCALING"},
        {26, "ENABLE USER WAIT AND PAUSE"},
	{27, "ENABLE PCONFIG"},
        {28, "ENABLE ENCLV EXITING"}};

/*
 * Tertiary Procbased controls
 */
struct capability_info tertiary_procbased[4] =
    {
        {0, "LOADIWKEY EXITING"},
        {1, "ENABLE HLAT"},
        {2, "EPT PAGING-WRITE CONTROL"},
        {3, "GUEST-PAGING VERIFICATION"}};

/*
 * report_capability
 *
 * Reports capabilities present in 'cap' using the corresponding MSR values
 * provided in 'low' and 'high'.
 *
 * Parameters:
 *  cap: capability_info structure for this feature
 *  length: number of entries in 'cap'
 *  low: low 32 bits of capability MSR value describing this feature
 *  high: high 32 bits of capability MSR value describing this feature
 */
void report_capability(struct capability_info *cap, uint8_t length, uint32_t low, uint32_t high)
{
    uint8_t i;
    struct capability_info *c;
    char mss[MAX_MSG];

    memset(mss, 0, sizeof(mss));

    for (i = 0; i < length; i++)
    {
        c = &cap[i];
        snprintf(mss, 79, "  %s: CAN SET=%s, CAN CLEAR=%s\n",
                 c->name,
                 (high & (1 << c->bit)) ? "Yes" : "No",
                 !(low & (1 << c->bit)) ? "Yes" : "No");
        printk(mss);
    }
}

/*
 * detect_vmx_features
 *
 * Detects and prints VMX capabilities of this host's CPU.
 */
void detect_vmx_features(void)
{
    uint32_t low, high;

    /* Pinbased controls */
    rdmsr(IA32_VMX_PINBASED_CTLS, low, high);
    pr_info("Pinbased Controls MSR: 0x%llx\n",
            (uint64_t)(low | (uint64_t)high << 32));
    report_capability(pinbased, 5, low, high);
    pr_info("\n");

    /* Procbased controls */
    rdmsr(IA32_VMX_PROCBASED_CTLS, low, high);
    pr_info("Procbased Controls MSR: 0x%llx\n",
            (uint64_t)(low | (uint64_t)high << 32));
    report_capability(procbased, 22, low, high);
    pr_info("\n");

    /* Secondary Procbased controls */
    rdmsr(IA32_VMX_PROCBASED_CTLS2, low, high);
    pr_info("Secondary Procbased Controls MSR: 0x%llx\n",
            (uint64_t)(low | (uint64_t)high << 32));
    report_capability(secondary_procbased, 28, low, high);
    pr_info("\n");

    /* Tertiary Procbased controls */
    rdmsr(IA32_VMX_PROCBASED_CTLS3, low, high);
    pr_info("Tertiary Procbased Controls MSR: 0x%llx\n",
            (uint64_t)(low | (uint64_t)high << 32));
    report_capability(tertiary_procbased, 4, low, high);
    pr_info("\n");

    /* Exit controls */
    rdmsr(IA32_VMX_EXIT_CTLS, low, high);
    pr_info("Exit Controls MSR: 0x%llx\n",
            (uint64_t)(low | (uint64_t)high << 32));
    report_capability(exit_controls, 16, low, high);
    pr_info("\n");

    /* Entry controls */
    rdmsr(IA32_VMX_ENTRY_CTLS, low, high);
    pr_info("Entry Controls MSR: 0x%llx\n",
            (uint64_t)(low | (uint64_t)high << 32));
    report_capability(entry_controls, 13, low, high);
    pr_info("\n");
}

/*
 * init_module
 *
 * Module entry point
 *
 * Return Values:
 *  Always 0
 */
int init_module(void)
{
    printk(KERN_INFO "CMPE 283 Assignment 1 Module Start\n");

    detect_vmx_features();

    /*
     * A non 0 return means init_module failed; module can't be loaded.
     */
    return 0;
}

/*
 * cleanup_module
 *
 * Function called on module unload
 */
void cleanup_module(void)
{
    printk(KERN_INFO "CMPE 283 Assignment 1 Module Exits\n");
}

MODULE_LICENSE("GPL");
