/*
 * camera.c
 *
 *  Created on: 2012-06-01
 *      Author: steve
 */

/* compile with gcc -Wall -o canon-capture -lgphoto2 canon-capture.c
 * This code released into the public domain 21 July 2008
 *
 * This program does the equivalent of:
 * gphoto2 --shell
 *   > set-config capture=1
 *   > capture-image-and-download
 * compile with gcc -Wall -o canon-capture -lgphoto2 canon-capture.c
 *
 * Taken from: http://credentiality2.blogspot.com/2008/07/linux-libgphoto2-image-capture-from.html
 */

//#include "config.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//x86
//#include </usr/local/include/gphoto2/gphoto2.h>
//arm
#include </usr/include/gphoto2/gphoto2.h>

static void liveview(Camera *canon, GPContext *canoncontext);
static void errordumper(GPLogLevel level, const char *domain,
		const char *format, va_list args, void *data);
static void capture(Camera *canon, GPContext *canoncontext);
static void shutterchange(Camera *canon, GPContext *canoncontext, int ss);
static void aperturechange(Camera *canon, GPContext *canoncontext, int ap);
static void isochange(Camera *canon, GPContext *canoncontext, int isoval);
int set_config_value_string(Camera *camera, const char *key, const char *val,
		GPContext *context);
static int _lookup_widget(CameraWidget*widget, const char *key,
		CameraWidget **child);
int canon_enable_capture(Camera *camera, int onoff, GPContext *context);

Camera *canon;
GPContext *canoncontext;

int cameraFunction(int command) {
	int i, retval, ss, commandint;

	gp_camera_new(&canon);
	canoncontext = gp_context_new();
	gp_log_add_func(GP_LOG_ERROR, errordumper, NULL);
	printf("Camera init.  Takes about 3 seconds.\n");
	retval = gp_camera_init(canon, canoncontext);
	if (retval != GP_OK) {
		printf("  Retval: %d\n", retval);
		exit(1);
	} else {
		printf("okay");
	}

	canon_enable_capture(canon, TRUE, canoncontext);

	while (1) {
		printf("ready to rock\n");

		//i = getchar();
		//getchar();
		//commandint = atoi(command);
		switch (command) {
		case 'c': {
			capture(canon, canoncontext);
			break;
		}
		case 's': {
			ss = getchar();
			getchar();
			printf("ss is %c\n", ss);
			shutterchange(canon, canoncontext, ss);
			break;
		}
		case 'a': {
			int ap;
			ap = getchar();
			getchar();
			aperturechange(canon, canoncontext, ap);
			printf("aperture: %d", ap);
			break;
		}
		case 'i': {
			int iso;
			iso = getchar();
			getchar();
			isochange(canon, canoncontext, iso);
			printf("iso: %d", iso);
			break;
		}
		case 'l': {
			while (1) {
				liveview(canon, canoncontext);
			}
			break;
		}
		case 'q': {
			gp_camera_exit(canon, canoncontext);
			break;
		}
		default:
			capture(canon, canoncontext);
			return 0;

		}
	}
	return 0;
} //end of code

static void liveview(Camera *canon, GPContext *canoncontext) {
	int retval;
	CameraFile *file;
	char output_file[64];

	retval = gp_file_new(&file);
	if (retval != GP_OK) {
		printf("  Retval: %d\n", retval);
		exit(1);
	}

	retval = gp_camera_capture_preview(canon, file, canoncontext);
	if (retval != GP_OK) {
		printf("  Retval: %d\n", retval);
		exit(1);
	}

	sprintf(output_file, "/dev/shm/liveviewpre.jpg");
	retval = gp_file_save(file, output_file);
	if (retval != GP_OK) {
		printf("  Retval: %d\n", retval);
		exit(1);
	}

	gp_file_unref(file);
}

static void errordumper(GPLogLevel level, const char *domain,
		const char *format, va_list args, void *data) {
	vfprintf(stdout, format, args);
	fprintf(stdout, "\n");
}

static void capture(Camera *canon, GPContext *canoncontext) {
	int retval, fd;
	CameraFile *canonfile;
	CameraFilePath camera_file_path;

	printf("Capturing.\n");

	retval = gp_camera_capture(canon, GP_CAPTURE_IMAGE, &camera_file_path,
			canoncontext);
	if (retval != GP_OK) {
		printf("  Retval: %d\n", retval);
		exit(1);
	}

	printf("Pathname on the camera: %s/%s\n", camera_file_path.folder,
			camera_file_path.name);

//	fd = open(camera_file_path.name, O_CREAT | O_WRONLY, 0644);
//	retval = gp_file_new_from_fd(&canonfile, fd);
//	printf("  Retval: %d\n", retval);
//	retval = gp_camera_file_get(canon, camera_file_path.folder, camera_file_path.name,
//		     GP_FILE_TYPE_NORMAL, canonfile, canoncontext);
//	printf("  Retval: %d\n", retval);
//
//
//	gp_file_free(canonfile);
}

static void shutterchange(Camera *canon, GPContext *canoncontext, int ss) {
	switch (ss) {
	case '1': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/250",
				canoncontext);
		//retval=gp_camera_set_config(canon,  , canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '2': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/200",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '3': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/160",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '4': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/125",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '5': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/100",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '6': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/80",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '7': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/60",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '8': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/40",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '9': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/20",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'q': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1/5",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'w': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "0.5",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'e': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "1",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'r': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "2",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 't': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "4",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'y': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "6",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'u': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "8",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'i': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "10",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'o': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "15",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'p': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "20",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '0': {
		int retval;
		printf("Shutter Speed.\n");
		retval = set_config_value_string(canon, "shutterspeed", "30",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	}
}
static void aperturechange(Camera *canon, GPContext *canoncontext, int ap) {
	switch (ap) {
	case '1': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "2.8",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '2': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "3.2",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '3': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "3.5",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '4': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "4", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '5': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "4.5",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '6': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "5", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '7': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "5.6",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '8': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "6.3",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '9': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "7.1",
				canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'q': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "8", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'w': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "9", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'e': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "10", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'r': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "11", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 't': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "13", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'y': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "14", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'u': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "16", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'i': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "18", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'o': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "20", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'p': {
		int retval;
		retval = set_config_value_string(canon, "aperture", "22", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}

	}
}

static void isochange(Camera *canon, GPContext *canoncontext, int isoval) {
	switch (isoval) {
	case '1': {
		int retval;
		retval = set_config_value_string(canon, "iso", "a", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '2': {
		int retval;
		retval = set_config_value_string(canon, "iso", "100", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '3': {
		int retval;
		retval = set_config_value_string(canon, "iso", "125", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '4': {
		int retval;
		retval = set_config_value_string(canon, "iso", "160", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '5': {
		int retval;
		retval = set_config_value_string(canon, "iso", "200", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '6': {
		int retval;
		retval = set_config_value_string(canon, "iso", "250", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '7': {
		int retval;
		retval = set_config_value_string(canon, "iso", "320", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '8': {
		int retval;
		retval = set_config_value_string(canon, "iso", "400", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '9': {
		int retval;
		retval = set_config_value_string(canon, "iso", "500", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'q': {
		int retval;
		retval = set_config_value_string(canon, "iso", "640", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'w': {
		int retval;
		retval = set_config_value_string(canon, "iso", "800", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'e': {
		int retval;
		retval = set_config_value_string(canon, "iso", "1000", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'r': {
		int retval;
		retval = set_config_value_string(canon, "iso", "1250", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 't': {
		int retval;
		retval = set_config_value_string(canon, "iso", "1600", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'y': {
		int retval;
		retval = set_config_value_string(canon, "iso", "2000", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'u': {
		int retval;
		retval = set_config_value_string(canon, "iso", "2500", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'i': {
		int retval;
		retval = set_config_value_string(canon, "iso", "3200", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'o': {
		int retval;
		retval = set_config_value_string(canon, "iso", "4000", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case 'p': {
		int retval;
		retval = set_config_value_string(canon, "iso", "5000", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}
	case '0': {
		int retval;
		retval = set_config_value_string(canon, "iso", "6400", canoncontext);
		printf("  Retval: %d\n", retval);
		break;
	}

	}
}

static int _lookup_widget(CameraWidget*widget, const char *key,
		CameraWidget **child) {
	int ret;
	ret = gp_widget_get_child_by_name(widget, key, child);
	if (ret < GP_OK)
		ret = gp_widget_get_child_by_label(widget, key, child);
	return ret;
}

int set_config_value_string(Camera *camera, const char *key, const char *val,
		GPContext *context) {
	CameraWidget *widget = NULL, *child = NULL;
	CameraWidgetType type;
	int ret;

	ret = gp_camera_get_config(camera, &widget, context);
	if (ret < GP_OK) {
		fprintf(stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	ret = _lookup_widget(widget, key, &child);
	if (ret < GP_OK) {
		fprintf(stderr, "lookup widget failed: %d\n", ret);
		goto out;
	}

	/* This type check is optional, if you know what type the label
	 * has already. If you are not sure, better check. */
	ret = gp_widget_get_type(child, &type);
	if (ret < GP_OK) {
		fprintf(stderr, "widget get type failed: %d\n", ret);
		goto out;
	}
	switch (type) {
	case GP_WIDGET_MENU:
	case GP_WIDGET_RADIO:
	case GP_WIDGET_TEXT:
	case GP_WIDGET_TOGGLE:
		break;
	default:
		fprintf(stderr, "widget has bad type %d\n", type);
		ret = GP_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* This is the actual set call. Note that we keep
	 * ownership of the string and have to free it if necessary.
	 */
	ret = gp_widget_set_value(child, val);
	if (ret < GP_OK) {
		fprintf(stderr, "could not set widget value: %d\n", ret);
		goto out;
	}
	/* This stores it on the camera again */
	ret = gp_camera_set_config(camera, widget, context);
	if (ret < GP_OK) {
		fprintf(stderr, "camera_set_config failed: %d\n", ret);
		return ret;
	}
	out: gp_widget_free(widget);
	return ret;
}

int canon_enable_capture(Camera *camera, int onoff, GPContext *context) {
	CameraWidget *widget = NULL, *child = NULL;
	CameraWidgetType type;
	int ret;

	ret = gp_camera_get_config(camera, &widget, context);
	if (ret < GP_OK) {
		fprintf(stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	ret = _lookup_widget(widget, "capture", &child);
	if (ret < GP_OK) {
		/*fprintf (stderr, "lookup widget failed: %d\n", ret);*/
		goto out;
	}

	ret = gp_widget_get_type(child, &type);
	if (ret < GP_OK) {
		fprintf(stderr, "widget get type failed: %d\n", ret);
		goto out;
	}
	switch (type) {
	case GP_WIDGET_TOGGLE:
		break;
	default:
		fprintf(stderr, "widget has bad type %d\n", type);
		ret = GP_ERROR_BAD_PARAMETERS;
		goto out;
	}
	/* Now set the toggle to the wanted value */
	ret = gp_widget_set_value(child, &onoff);
	if (ret < GP_OK) {
		fprintf(stderr, "toggling Canon capture to %d failed with %d\n", onoff,
				ret);
		goto out;
	}
	/* OK */
	ret = gp_camera_set_config(camera, widget, context);
	if (ret < GP_OK) {
		fprintf(stderr, "camera_set_config failed: %d\n", ret);
		return ret;
	}
	out: gp_widget_free(widget);
	return ret;
}

