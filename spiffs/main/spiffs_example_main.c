/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "example";
char * TempName =  "Dick";

typedef struct DICKTEST
{
	char  * Name ;
	unsigned char   age;
	unsigned char  high;
} Dicktest;


void app_main(void)
{   
	 char line[200] = {0xff};
	 char readline[100] = {0};
	 char *pstr = NULL;
	Dicktest  Tempinformation[2];
	 Dicktest information[2];

	 information[0].Name = "ZengZhi";
	 information[0].age = 30;
	 information[0].high = 170;

	 information[1].Name = "Dick";
	 information[1].high = 180;
	 information[1].age = 28;

		
		
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };
    
    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
	FILE* d = fopen("/spiffs/Dick.txt", "w");
    if (d == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
   // fprintf(f, "Dick test spiffs . Hello World!\n");
	fwrite(&information, sizeof(char),sizeof(information), f);
	fclose(f);
	
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) {
        // Delete it if it exists
        unlink("/spiffs/foo.txt");
    }
    
    // Rename original file
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // Open renamed file for reading.
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }

 //   fgets(line, sizeof(line), f);
    int len = fread(&Tempinformation,sizeof(char),sizeof(line),f);
    fclose(f);
    ESP_LOGI(TAG, "Read from file:  name =%s, age = %d, high =%d  ",Tempinformation[0].Name, Tempinformation[0].age,Tempinformation[0].high);
	
    ESP_LOGI(TAG, "Read from file:  name =%s, age = %d, high =%d  ",Tempinformation[1].Name, Tempinformation[1].age,Tempinformation[1].high);
    //ESP_LOGI(TAG, "Read from file: '%s'",pstr);
    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}
