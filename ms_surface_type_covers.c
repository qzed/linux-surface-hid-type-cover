#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/hid.h>

#define USB_VENDOR_ID_MICROSOFT			0x045e
#define USB_DEVICE_ID_MS_TYPE_COVER_PRO_5_1	0x09C0



static int mstc_ll_start(struct hid_device *hdev)
{
	struct hid_device *orig = hdev->driver_data;

	hid_info(hdev, "mstc_ll_start\n");

	orig->collection = hdev->collection;
	orig->collection_size = hdev->collection_size;
	orig->maxcollection = hdev->maxcollection;
	orig->maxapplication = hdev->maxapplication;
	return hid_hw_start(orig, HID_CONNECT_DEFAULT);
}

static void mstc_ll_stop(struct hid_device *hdev)
{
	hid_info(hdev, "mstc_ll_stop\n");

	hid_hw_stop(hdev->driver_data);
}

static int mstc_ll_open(struct hid_device *hdev)
{
	hid_info(hdev, "mstc_ll_open\n");

	return hid_hw_open(hdev->driver_data);
}

static void mstc_ll_close(struct hid_device *hdev)
{
	hid_info(hdev, "mstc_ll_close\n");

	hid_hw_close(hdev->driver_data);
}

static int mstc_ll_power(struct hid_device *hdev, int level)
{
	hid_info(hdev, "mstc_ll_power\n");

	return hid_hw_power(hdev->driver_data, level);
}

static int mstc_ll_parse(struct hid_device *hdev)
{
	struct hid_device *orig = hdev->driver_data;
	int status;

	hid_info(hdev, "mstc_ll_parse\n");

	if (orig->ll_driver->parse) {
		status = orig->ll_driver->parse(orig);
		if (status) {
			return status;
		}

		hdev->dev_rdesc = orig->dev_rdesc;
		hdev->dev_rsize = orig->dev_rsize;
	}

	return 0;
}

static void mstc_ll_request(struct hid_device *hdev, struct hid_report *report, int reqtype)
{
	hid_info(hdev, "mstc_ll_request\n");

	hid_hw_request(hdev->driver_data, report, reqtype);
}

static int mstc_ll_wait(struct hid_device *hdev)
{
	hid_info(hdev, "mstc_ll_wait\n");

	hid_hw_wait(hdev->driver_data);
	return 0;
}

static int mstc_ll_raw_request(struct hid_device *hdev, unsigned char reportnum,
			       __u8 *buf, size_t len, unsigned char rtype, int reqtype)
{
	hid_info(hdev, "mstc_ll_raw_request\n");

	return hid_hw_raw_request(hdev->driver_data, reportnum, buf, len, rtype, reqtype);
}

static int mstc_ll_output_report(struct hid_device *hdev, __u8 *buf, size_t len)
{
	hid_info(hdev, "mstc_ll_output_report\n");

	return hid_hw_output_report(hdev->driver_data, buf, len);
}

static int mstc_ll_idle(struct hid_device *hdev, int report, int idle, int reqtype)
{
	hid_info(hdev, "mstc_ll_idle\n");

	return hid_hw_idle(hdev->driver_data, report, idle, reqtype);
}

struct hid_ll_driver mstc_ll_driver = {
	.start         = mstc_ll_start,
	.stop          = mstc_ll_stop,
	.open          = mstc_ll_open,
	.close         = mstc_ll_close,
	.power         = mstc_ll_power,
	.parse         = mstc_ll_parse,
	.request       = mstc_ll_request,
	.wait          = mstc_ll_wait,
	.raw_request   = mstc_ll_raw_request,
	.output_report = mstc_ll_output_report,
	.idle          = mstc_ll_idle,
};


static int mstc_raw_event(struct hid_device *hdev, struct hid_report *report, u8 *data, int size)
{
	struct hid_device *clone = hid_get_drvdata(hdev);
	return hid_input_report(hdev, report->type, data, size, 1);
}

static int mstc_event(struct hid_device *hdev, struct hid_field *field,
		      struct hid_usage *usage, __s32 value)
{
	return 1;
}


static int mstc_suspend(struct hid_device *hdev, pm_message_t message)
{
	hid_info(hdev, "mstc_suspend\n");
	return 0;
}

static int mstc_resume(struct hid_device *hdev)
{
	hid_info(hdev, "mstc_resume\n");
	return 0;
}

static int mstc_reset_resume(struct hid_device *hdev)
{
	hid_info(hdev, "mstc_reset_resume\n");
	return 0;
}


static struct hid_device *mstc_create_hid_device(struct hid_device *hdev)
{
	struct hid_device *clone;

	clone = hid_allocate_device();
	if (IS_ERR(clone)) {
		return clone;
	}

	clone->ll_driver = &mstc_ll_driver;
	clone->driver_data = hdev;

	clone->dev.parent = hdev->dev.parent;
	clone->group = hdev->group;
	clone->vendor = hdev->vendor;
	clone->product = hdev->product;
	clone->version = hdev->version;
	clone->type = hdev->type;
	clone->country = hdev->country;
	strlcpy(clone->name, hdev->name, sizeof(clone->name));
	strlcpy(clone->phys, hdev->phys, sizeof(clone->phys));

	clone->bus = BUS_VIRTUAL;

	return clone;
}

static int mstc_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	struct hid_device *clone;
	int status = 0;

	hid_info(hdev, "msct_probe: device: %s, bus: %d\n", hdev->name, hdev->bus);

	clone = mstc_create_hid_device(hdev);
	if (IS_ERR(clone)) {
		status = PTR_ERR(clone);
		goto err_create;
	}

	status = hid_add_device(clone);
	if (status) {
		goto err_add;
	}

	hid_set_drvdata(hdev, clone);

	hid_info(hdev, "msct_probe: success");
	return 0;

err_add:
	hid_destroy_device(clone);
err_create:
	return status;
}

static void mstc_remove(struct hid_device *hdev)
{
	struct hid_device *clone = hid_get_drvdata(hdev);

	hid_info(hdev, "mstc_remove: device: %s, bus: %d\n", hdev->name, hdev->bus);

	hid_set_drvdata(hdev, NULL);
	hid_destroy_device(clone);
}


static const struct hid_device_id ms_surface_type_cover_ids[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_TYPE_COVER_PRO_5_1), 0 },
	{ },
};

MODULE_DEVICE_TABLE(hid, ms_surface_type_cover_ids);


struct hid_driver ms_surface_type_cover_driver = {
	.name         = "hid_ms_surface_type_cover",
	.id_table     = ms_surface_type_cover_ids,

	.probe        = mstc_probe,
	.remove       = mstc_remove,
	.raw_event    = mstc_raw_event,
	.event        = mstc_event,

	.suspend      = mstc_suspend,
	.resume       = mstc_resume,
	.reset_resume = mstc_reset_resume,
};

module_hid_driver(ms_surface_type_cover_driver);

MODULE_AUTHOR("Maximilian Luz");
MODULE_DESCRIPTION("Driver for Surface Type Covers");
MODULE_LICENSE("GPL v2");
