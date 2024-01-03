#include <config/kube_config.h>
#include <api/AppsV1API.h>
#include <model/object.h>
#include <stdio.h>

void patch_deployment(apiClient_t *apiClient)
{
    v1_deployment_t *deploy;
    object_t * body;

    cJSON *jsonArray = cJSON_CreateArray();
    cJSON *jsonObject = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonObject, "op", "replace");
    cJSON_AddStringToObject(jsonObject, "path", "/spec/replicas");
    cJSON_AddNumberToObject(jsonObject, "value", 2);

    cJSON_AddItemToArray(jsonArray, jsonObject);

    body = object_parseFromJSON(jsonArray);
    if (! body) {
        fprintf(stderr, "failed to convert patch to object\n");
        return;
    }

    v1_scale_t *result = AppsV1API_patchNamespacedDeployment(
            apiClient,
            "web",
            "default",
            body,
            NULL,
            NULL,
            NULL,
            NULL,
            0);

    if (apiClient->response_code != 200) {
        fprintf(stderr, "failed to patch resource: %ld\n", apiClient->response_code);
    }
}

int main()
{
    char *basePath = NULL;
    sslConfig_t *sslConfig = NULL;
    list_t *apiKeys = NULL;
    int rc = load_kube_config(&basePath, &sslConfig, &apiKeys, NULL); /* NULL means loading configuration from $HOME/.kube/config */
    if (rc != 0)
    {
        printf("Cannot load kubernetes configuration.\n");
        return -1;
    }
    apiClient_t *apiClient = apiClient_create_with_base_path(basePath, sslConfig, apiKeys);
    if (!apiClient)
    {
        printf("Cannot create a kubernetes client.\n");
        return -1;
    }

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
