#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/hid.h>

#define USB_VENDOR_ID_MICROSOFT			0x045e
#define USB_DEVICE_ID_MS_TYPE_COVER_PRO_5_1	0x09C0



static int mstc_raw_event(struct hid_device *hdev, struct hid_report *report, u8 *data, int size)
{
	return 0;	// TODO
}

static int mstc_event(struct hid_device *hdev, struct hid_field *field,
		      struct hid_usage *usage, __s32 value)
{
	return 0;	// TODO
}

static void mstc_report(struct hid_device *hdev, struct hid_report *report)
{
	// TODO
}

static __u8 *mstc_report_fixup(struct hid_device *hdev, __u8 *buf, unsigned int *size)
{
	return buf;	// TODO
}

static int mstc_input_mapping(struct hid_device *hdev, struct hid_input *hidinput,
			      struct hid_field *field, struct hid_usage *usage,
			      unsigned long **bit, int *max)
{
	return 0;	// TODO
}

static int mstc_input_mapped(struct hid_device *hdev, struct hid_input *hidinput,
			     struct hid_field *field, struct hid_usage *usage,
			     unsigned long **but, int *max)
{
	return 0;	// TODO
}

static int mstc_input_configured(struct hid_device *hdev, struct hid_input *hidinput)
{
	return 0;	// TODO
}

static void mstc_feature_mapping(struct hid_device *hdev, struct hid_field *field,
				 struct hid_usage *usage)
{
	// TODO
}


static int mstc_suspend(struct hid_device *hdev, pm_message_t message)
{
	return 0;	// TODO
}

static int mstc_resume(struct hid_device *hdev)
{
	return 0;	// TODO
}

static int mstc_reset_resume(struct hid_device *hdev)
{
	return 0;	// TODO
}


static int mstc_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	return 0;	// TODO
}

static void mstc_remove(struct hid_device *hdev)
{
	// TODO
}


static const struct hid_device_id ms_surface_type_cover_ids[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_MICROSOFT, USB_DEVICE_ID_MS_TYPE_COVER_PRO_5_1), 0 },
	{ },
};

MODULE_DEVICE_TABLE(hid, ms_surface_type_cover_ids);


struct hid_driver ms_surface_type_cover_driver = {
	.name             = "ms_surface_type_cover_hid",
	.id_table         = ms_surface_type_cover_ids,

	.probe            = mstc_probe,
	.remove           = mstc_remove,
	.raw_event        = mstc_raw_event,
	.event            = mstc_event,
	.report           = mstc_report,
	.report_fixup     = mstc_report_fixup,
	.input_mapping    = mstc_input_mapping,
	.input_mapped     = mstc_input_mapped,
	.input_configured = mstc_input_configured,
	.feature_mapping  = mstc_feature_mapping,

	.suspend       = mstc_suspend,
	.resume        = mstc_resume,
	.reset_resume  = mstc_reset_resume,
};

module_hid_driver(ms_surface_type_cover_driver);
