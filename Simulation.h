#include<stdio.h>
#define STACK_SIZE 1024
#define MAX_ARG_LEN 100
typedef enum {
	READY,
    RUNNING,
	BLOCKED,
	DEADLOCKED
} process_state_t;

typedef enum {
	APPLICATION,
	SYSTEM
} process_class_t;

/* Process control block structure */
typedef struct {
	char pid[20]; 
	/* Process class (differentiates applications from system processes. */
	//process_class_t class;
	/* Process priority. Higher numerical value = higher priority.
	 * Valid values are -128 through 127 (inclusive). */
	int priority;
	/* Process state (Ready, Running, or Blocked). */
	process_state_t state;
	/* Pointer to the stack_top of this processes's stack. */
	unsigned char *stack_top;
	/* Pointer to the bottom of this processes's stack. */
	unsigned char *stack_base;
	/* Array to store messages for sending */
	char message[1000];
	/* Memory size ... will be used in R3 and R4. */
	//int memory_size;
	/* Load address ... will be used in R3 and R4. */
	//unsigned char *load_address;
	/* Execution address ... will be used in R3 and R4. */
	//unsigned char *exec_address;
} pcb_t;

/* Enum constants for process sort order (i.e., queue insertion order.) */
typedef enum {
	FIFO,
	PRIORITY
} pcb_queue_sort_order_t;


/* PCB queue node; points to a single PCB that is associated with this node. */
typedef struct pcb_queue_node {
	/* Pointer to the next PCB node in the queue. */
	struct pcb_queue_node *next;
	/* Pointer to the previous PCB node in the queue. */
	struct pcb_queue_node *prev;
	/* Pointer to the actual PCB associated with this node. */
	pcb_t *pcb;
} pcb_queue_node_t;


/* PCB queue; represents a queue of processes. */
typedef struct pcb_queue {
	/* Pointer to the first element in the queue. */
	pcb_queue_node_t *head;
	/* Pointer to the last element in the queue. */
	pcb_queue_node_t *tail;
	/* Number of elements in the queue. */
	unsigned int length;
	/* Specifies how elements in this queue are sorted at insert-time. */
	pcb_queue_sort_order_t sort_order;
} pcb_queue_t;

void init_pcb_queues		( void );
pcb_queue_t* get_queue_by_state	( process_state_t state );
pcb_t* setup_pcb   ( char *name, int priority, process_class_t class );
pcb_t* find_pcb ( char name[20],
	pcb_queue_t *queues );
pcb_queue_t* remove_pcb	( pcb_t *pcb );
pcb_queue_t* insert_pcb	( pcb_t *pcb );
int	block_pcb ( pcb_t *pcb );
int	unblock_pcb ( pcb_t *pcb );
int	suspend_pcb	( pcb_t *pcb );
int	resume_pcb	( pcb_t *pcb );
int	is_blocked	( pcb_t *pcb );
int	is_suspended ( pcb_t *pcb );
int	is_ready ( pcb_t *pcb );
char* process_state_to_string ( process_state_t state );
char* process_class_to_string ( process_class_t class );
char process_class_to_char ( process_class_t class );