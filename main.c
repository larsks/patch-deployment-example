#include <config/kube_config.h>
#include <api/CoreV1API.h>
#include <api/AppsV1API.h>
#include <stdio.h>

void list_pod(apiClient_t * apiClient)
{
    v1_pod_list_t *pod_list = NULL;
    pod_list = CoreV1API_listNamespacedPod(apiClient, "default",    /*namespace */
                                           NULL,    /* pretty */
                                           NULL,    /* allowWatchBookmarks */
                                           NULL,    /* continue */
                                           NULL,    /* fieldSelector */
                                           NULL,    /* labelSelector */
                                           NULL,    /* limit */
                                           NULL,    /* resourceVersion */
                                           NULL,    /* resourceVersionMatch */
                                           NULL,    /* sendInitialEvents */
                                           NULL,    /* timeoutSeconds */
                                           NULL /* watch */
        );
    printf("The return code of HTTP request=%ld\n", apiClient->response_code);
    if (pod_list) {
        printf("Get pod list:\n");
        listEntry_t *listEntry = NULL;
        v1_pod_t *pod = NULL;
        list_ForEach(listEntry, pod_list->items) {
            pod = listEntry->data;
            printf("\tThe pod name: %s\n", pod->metadata->name);
        }
        v1_pod_list_free(pod_list);
        pod_list = NULL;
    } else {
        printf("Cannot get any pod.\n");
    }
}

void patch_deployment(apiClient_t * apiClient) {
    v1_deployment_t *deploy;
    char *name = "web";
    char *namespace = "default";
    cJSON *patch;
    object_t * patcho;

    patch = cJSON_ParseWithOpts("{\"spec\": {\"replicas\": 2}}", NULL, 1);
    patcho = cJSON_ToObject(patch);

    deploy = AppsV1API_patchNamespacedDeployment(
            apiClient,
            name,
            namespace,
            patch,
            NULL,
            NULL,
            "exampleclient",
            "strict",
            0);
}

int main()
{
    char *basePath = NULL;
    sslConfig_t *sslConfig = NULL;
    list_t *apiKeys = NULL;
    int rc = load_kube_config(&basePath, &sslConfig, &apiKeys, NULL);   /* NULL means loading configuration from $HOME/.kube/config */
    if (rc != 0) {
        printf("Cannot load kubernetes configuration.\n");
        return -1;
    }
    apiClient_t *apiClient = apiClient_create_with_base_path(basePath, sslConfig, apiKeys);
    if (!apiClient) {
        printf("Cannot create a kubernetes client.\n");
        return -1;
    }

    //list_pod(apiClient);
    patch_deployment(apiClient);

    apiClient_free(apiClient);
    apiClient = NULL;
    free_client_config(basePath, sslConfig, apiKeys);
    basePath = NULL;
    sslConfig = NULL;
    apiKeys = NULL;
    apiClient_unsetupGlobalEnv();

    return 0;
}
