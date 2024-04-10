#ifndef __APP_TASKS_H
#define __APP_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void app_create_tasks(void);
void set_log_pass(const char* username, const char* password);
void run_mvc(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_TASKS_H */
