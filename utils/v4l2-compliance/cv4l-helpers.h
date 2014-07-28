#ifndef _CV4L_HELPERS_H_
#define _CV4L_HELPERS_H_

#include <limits.h>
#include <v4l-helpers.h>

#define cv4l_ioctl(cmd, arg) v4l_named_ioctl(g_v4l_fd(), #cmd, cmd, arg)

class cv4l_fd : v4l_fd {
public:
	cv4l_fd()
	{
		v4l_fd_init(this);
	}
	cv4l_fd(cv4l_fd *fd)
	{
		*this = *fd;
	}

	__u32 g_type() const { return type; }
	void s_type(__u32 type) { v4l_s_type(this, type); }
	__u32 g_selection_type() const { return v4l_g_selection_type(this); }
	__u32 g_caps() const { return caps; }
	int g_fd() const { return fd; }
	v4l_fd *g_v4l_fd() { return this; }
	bool g_direct() const { return v4l_fd_g_direct(this); }
	void s_direct(bool direct) { v4l_fd_s_direct(this, direct); }
	bool g_trace() const { return v4l_fd_g_trace(this); }
	void s_trace(bool trace) { v4l_fd_s_trace(this, trace); }

	int open(const char *devname, bool non_blocking = false) { return v4l_open(this, devname, non_blocking); }
	int close() { return v4l_close(this); }
	int reopen(bool non_blocking = false) { return v4l_reopen(this, non_blocking); }
	ssize_t read(void *buffer, size_t n) { return v4l_read(this, buffer, n); }
	ssize_t write(const void *buffer, size_t n) { return v4l_write(this, buffer, n); }
	void *mmap(size_t length, off_t offset) { return v4l_mmap(this, length, offset); }
	int munmap(void *start, size_t length) { return v4l_munmap(this, start, length); }

	bool has_vid_cap() const { return v4l_has_vid_cap(this); }
	bool has_vid_out() const { return v4l_has_vid_out(this); }
	bool has_vid_m2m() const { return v4l_has_vid_m2m(this); }
	bool has_vid_mplane() const { return v4l_has_vid_mplane(this); }
	bool has_overlay_cap() const { return v4l_has_overlay_cap(this); }
	bool has_overlay_out() const { return v4l_has_overlay_out(this); }
	bool has_raw_vbi_cap() const { return v4l_has_raw_vbi_cap(this); }
	bool has_sliced_vbi_cap() const { return v4l_has_sliced_vbi_cap(this); }
	bool has_vbi_cap() const { return v4l_has_vbi_cap(this); }
	bool has_raw_vbi_out() const { return v4l_has_raw_vbi_out(this); }
	bool has_sliced_vbi_out() const { return v4l_has_sliced_vbi_out(this); }
	bool has_vbi_out() const { return v4l_has_vbi_out(this); }
	bool has_vbi() const { return v4l_has_vbi(this); }
	bool has_radio_rx() const { return v4l_has_radio_rx(this); }
	bool has_radio_tx() const { return v4l_has_radio_tx(this); }
	bool has_rds_cap() const { return v4l_has_rds_cap(this); }
	bool has_rds_out() const { return v4l_has_rds_out(this); }
	bool has_sdr_cap() const { return v4l_has_sdr_cap(this); }
	bool has_hwseek() const { return v4l_has_hwseek(this); }
	bool has_rw() const { return v4l_has_rw(this); }
	bool has_streaming() const { return v4l_has_streaming(this); }
	bool has_ext_pix_format() const { return v4l_has_ext_pix_format(this); }

	int queryctrl(v4l2_queryctrl &qc)
	{
		return cv4l_ioctl(VIDIOC_QUERYCTRL, &qc);
	}

	int querymenu(v4l2_querymenu &qm)
	{
		return cv4l_ioctl(VIDIOC_QUERYMENU, &qm);
	}

	int query_ext_ctrl(v4l2_query_ext_ctrl &qec, bool next_ctrl = false, bool next_compound = false)
	{
		return v4l_query_ext_ctrl(this, &qec, next_ctrl, next_compound);
	}

	int g_ctrl(v4l2_control &ctrl)
	{
		return cv4l_ioctl(VIDIOC_G_CTRL, &ctrl);
	}

	int s_ctrl(v4l2_control &ctrl)
	{
		return cv4l_ioctl(VIDIOC_S_CTRL, &ctrl);
	}

	int g_ext_ctrls(v4l2_ext_controls &ec)
	{
		return v4l_g_ext_ctrls(this, &ec);
	}

	int try_ext_ctrls(v4l2_ext_controls &ec)
	{
		return v4l_try_ext_ctrls(this, &ec);
	}

	int s_ext_ctrls(v4l2_ext_controls &ec)
	{
		return v4l_s_ext_ctrls(this, &ec);
	}

	int g_fmt(v4l2_format &fmt, unsigned type = 0)
	{
		return v4l_g_fmt(this, &fmt, type);
	}

	int try_fmt(v4l2_format &fmt)
	{
		return v4l_try_fmt(this, &fmt);
	}

	int s_fmt(v4l2_format &fmt)
	{
		return v4l_s_fmt(this, &fmt);
	}

	int g_selection(v4l2_selection &sel)
	{
		return v4l_g_selection(this, &sel);
	}

	int s_selection(v4l2_selection &sel)
	{
		return v4l_s_selection(this, &sel);
	}

	int g_tuner(v4l2_tuner &tuner, unsigned index = 0)
	{
		memset(&tuner, 0, sizeof(tuner));
		tuner.index = index;
		int ret = cv4l_ioctl(VIDIOC_G_TUNER, &tuner);
		if (ret == 0 && tuner.rangehigh > INT_MAX)
			tuner.rangehigh = INT_MAX;
		return ret;
	}

	int s_tuner(v4l2_tuner &tuner)
	{
		return cv4l_ioctl(VIDIOC_S_TUNER, &tuner);
	}

	int g_modulator(v4l2_modulator &modulator)
	{
		memset(&modulator, 0, sizeof(modulator));
		return cv4l_ioctl(VIDIOC_G_MODULATOR, &modulator);
	}

	int s_modulator(v4l2_modulator &modulator)
	{
		return cv4l_ioctl(VIDIOC_S_MODULATOR, &modulator);
	}

	int enum_input(v4l2_input &in, bool init = false, int index = 0)
	{
		if (init) {
			memset(&in, 0, sizeof(in));
			in.index = index;
		} else {
			in.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUMINPUT, &in);
	}

	int enum_output(v4l2_output &out, bool init = false, int index = 0)
	{
		if (init) {
			memset(&out, 0, sizeof(out));
			out.index = index;
		} else {
			out.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUMOUTPUT, &out);
	}

	int enum_audio(v4l2_audio &audio, bool init = false, int index = 0)
	{
		if (init) {
			memset(&audio, 0, sizeof(audio));
			audio.index = index;
		} else {
			audio.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUMAUDIO, &audio);
	}

	int enum_audout(v4l2_audioout &audout, bool init = false, int index = 0)
	{
		if (init) {
			memset(&audout, 0, sizeof(audout));
			audout.index = index;
		} else {
			audout.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUMAUDOUT, &audout);
	}

	bool has_crop()
	{
		v4l2_selection sel;

		memset(&sel, 0, sizeof(sel));
		sel.type = g_selection_type();
		sel.target = V4L2_SEL_TGT_CROP;
		return g_selection(sel) != ENOTTY;
	}

	bool has_compose()
	{
		v4l2_selection sel;

		memset(&sel, 0, sizeof(sel));
		sel.type = g_selection_type();
		sel.target = V4L2_SEL_TGT_COMPOSE;
		return g_selection(sel) != ENOTTY;
	}

	bool cur_io_has_crop()
	{
		v4l2_selection sel;

		memset(&sel, 0, sizeof(sel));
		sel.type = g_selection_type();
		sel.target = V4L2_SEL_TGT_CROP;
		return g_selection(sel) == 0;
	}

	bool cur_io_has_compose()
	{
		v4l2_selection sel;

		memset(&sel, 0, sizeof(sel));
		sel.type = g_selection_type();
		sel.target = V4L2_SEL_TGT_COMPOSE;
		return g_selection(sel) == 0;
	}

	int subscribe_event(v4l2_event_subscription &sub)
	{
		return cv4l_ioctl(VIDIOC_SUBSCRIBE_EVENT, &sub);
	}

	int dqevent(v4l2_event &ev)
	{
		return cv4l_ioctl(VIDIOC_DQEVENT, &ev);
	}

	int g_input(__u32 &input)
	{
		return cv4l_ioctl(VIDIOC_G_INPUT, &input);
	}

	int s_input(__u32 input)
	{
		return cv4l_ioctl(VIDIOC_S_INPUT, &input);
	}

	int g_output(__u32 &output)
	{
		return cv4l_ioctl(VIDIOC_G_OUTPUT, &output);
	}

	int s_output(__u32 output)
	{
		return cv4l_ioctl(VIDIOC_S_OUTPUT, &output);
	}

	int g_audio(v4l2_audio &audio)
	{
		memset(&audio, 0, sizeof(audio));
		return cv4l_ioctl(VIDIOC_G_AUDIO, &audio);
	}

	int s_audio(__u32 input)
	{
		v4l2_audio audio;

		memset(&audio, 0, sizeof(audio));
		audio.index = input;
		return cv4l_ioctl(VIDIOC_S_AUDIO, &audio);
	}

	int g_audout(v4l2_audioout &audout)
	{
		memset(&audout, 0, sizeof(audout));
		return cv4l_ioctl(VIDIOC_G_AUDOUT, &audout);
	}

	int s_audout(__u32 output)
	{
		v4l2_audioout audout;

		memset(&audout, 0, sizeof(audout));
		audout.index = output;
		return cv4l_ioctl(VIDIOC_S_AUDOUT, &audout);
	}

	int g_std(v4l2_std_id &std)
	{
		return cv4l_ioctl(VIDIOC_G_STD, &std);
	}

	int s_std(v4l2_std_id std)
	{
		return cv4l_ioctl(VIDIOC_S_STD, &std);
	}

	int query_std(v4l2_std_id &std)
	{
		return cv4l_ioctl(VIDIOC_QUERYSTD, &std);
	}

	int g_dv_timings(v4l2_dv_timings &timings)
	{
		return cv4l_ioctl(VIDIOC_G_DV_TIMINGS, &timings);
	}

	int s_dv_timings(v4l2_dv_timings &timings)
	{
		return cv4l_ioctl(VIDIOC_S_DV_TIMINGS, &timings);
	}

	int query_dv_timings(v4l2_dv_timings &timings)
	{
		return cv4l_ioctl(VIDIOC_QUERY_DV_TIMINGS, &timings);
	}

	int g_frequency(v4l2_frequency &freq, unsigned index = 0)
	{
		memset(&freq, 0, sizeof(freq));
		freq.tuner = index;
		freq.type = V4L2_TUNER_ANALOG_TV;
		return cv4l_ioctl(VIDIOC_G_FREQUENCY, &freq);
	}

	int s_frequency(v4l2_frequency &freq)
	{
		return cv4l_ioctl(VIDIOC_S_FREQUENCY, &freq);
	}

	int g_priority(__u32 &prio)
	{
		return cv4l_ioctl(VIDIOC_G_PRIORITY, &prio);
	}

	int s_priority(__u32 prio = V4L2_PRIORITY_DEFAULT)
	{
		return cv4l_ioctl(VIDIOC_S_PRIORITY, &prio);
	}

	int streamon(__u32 type = 0)
	{
		if (type == 0)
			type = g_type();
		return cv4l_ioctl(VIDIOC_STREAMON, &type);
	}

	int streamoff(__u32 type = 0)
	{
		if (type == 0)
			type = g_type();
		return cv4l_ioctl(VIDIOC_STREAMOFF, &type);
	}

	int querybuf(v4l_buffer &buf, unsigned index)
	{
		return v4l_buffer_querybuf(this, &buf, index);
	}

	int dqbuf(v4l_buffer &buf)
	{
		return v4l_buffer_dqbuf(this, &buf);
	}

	int qbuf(v4l_buffer &buf)
	{
		return v4l_buffer_qbuf(this, &buf);
	}

	int prepare_buf(v4l_buffer &buf)
	{
		return v4l_buffer_prepare_buf(this, &buf);
	}

	int enum_std(v4l2_standard &std, bool init = false, int index = 0)
	{
		if (init) {
			memset(&std, 0, sizeof(std));
			std.index = index;
		} else {
			std.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUMSTD, &std);
	}

	int enum_dv_timings(v4l2_enum_dv_timings &timings, bool init = false, int index = 0)
	{
		if (init) {
			memset(&timings, 0, sizeof(timings));
			timings.index = index;
		} else {
			timings.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUM_DV_TIMINGS, &timings);
	}

	int enum_fmt(v4l2_fmtdesc &fmt, bool init = false, int index = 0, unsigned type = 0)
	{
		if (init) {
			memset(&fmt, 0, sizeof(fmt));
			fmt.index = index;
		} else {
			fmt.index++;
		}
		fmt.type = type ? type : g_type();
		return cv4l_ioctl(VIDIOC_ENUM_FMT, &fmt);
	}

	int enum_framesizes(v4l2_frmsizeenum &frm, __u32 init_pixfmt = 0, int index = 0)
	{
		if (init_pixfmt) {
			memset(&frm, 0, sizeof(frm));
			frm.pixel_format = init_pixfmt;
			frm.index = index;
		} else {
			frm.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUM_FRAMESIZES, &frm);
	}

	int enum_frameintervals(v4l2_frmivalenum &frm, __u32 init_pixfmt = 0, __u32 w = 0, __u32 h = 0, int index = 0)
	{
		if (init_pixfmt) {
			memset(&frm, 0, sizeof(frm));
			frm.pixel_format = init_pixfmt;
			frm.width = w;
			frm.height = h;
			frm.index = index;
		} else {
			frm.index++;
		}
		return cv4l_ioctl(VIDIOC_ENUM_FRAMEINTERVALS, &frm);
	}

	int set_interval(v4l2_fract interval, unsigned type = 0)
	{
		v4l2_streamparm parm;

		parm.type = type ? type : g_type();
		if (cv4l_ioctl(VIDIOC_G_PARM, &parm) ||
		    !(parm.parm.capture.capability & V4L2_CAP_TIMEPERFRAME))
			return -1;

		parm.parm.capture.timeperframe = interval;

		return cv4l_ioctl(VIDIOC_S_PARM, &parm);
	}

	int get_interval(v4l2_fract &interval, unsigned type = 0)
	{
		v4l2_streamparm parm;

		parm.type = type ? type : g_type();
		if (cv4l_ioctl(VIDIOC_G_PARM, &parm) == 0 &&
		    (parm.parm.capture.capability & V4L2_CAP_TIMEPERFRAME)) {
			interval = parm.parm.capture.timeperframe;
			return 0;
		}

		return -1;
	}

	v4l2_fract g_pixel_aspect(unsigned &width, unsigned &height, unsigned type = 0)
	{
		v4l2_cropcap ratio;
		v4l2_dv_timings timings;
		v4l2_std_id std;
		static const v4l2_fract square = { 1, 1 };
		static const v4l2_fract hz50 = { 11, 12 };
		static const v4l2_fract hz60 = { 11, 10 };

		ratio.type = type ? type : g_selection_type();
		if (cv4l_ioctl(VIDIOC_CROPCAP, &ratio) == 0) {
			width = ratio.defrect.width;
			height = ratio.defrect.height;
			if (ratio.pixelaspect.numerator && ratio.pixelaspect.denominator)
				return ratio.pixelaspect;
		}

		width = 720;
		height = 480;
		if (!g_std(std)) {
			if (std & V4L2_STD_525_60)
				return hz60;
			if (std & V4L2_STD_625_50) {
				height = 576;
				return hz50;
			}
		}

		if (!g_dv_timings(timings)) {
			width = timings.bt.width;
			height = timings.bt.height;
			if (width == 720 && height == 480)
				return hz60;
			if (width == 720 && height == 576) {
				height = 576;
				return hz50;
			}
			return square;
		}
		width = 0;
		height = 0;
		return square;
	}
};

class cv4l_fmt : public v4l2_format {
public:
	cv4l_fmt(unsigned _type = 0)
	{
		v4l_format_init(this, _type);
	}
	cv4l_fmt(const v4l2_format &_fmt)
	{
		*this = _fmt;
	}

	__u32 g_type() { return type; }
	void s_type(unsigned type) { v4l_format_init(this, type); }
	__u32 g_width() { return v4l_format_g_width(this); }
	void s_width(__u32 width) { v4l_format_s_width(this, width); }
	__u32 g_height() { return v4l_format_g_height(this); }
	void s_height(__u32 height) { v4l_format_s_height(this, height); }
	__u32 g_pixelformat() { return v4l_format_g_pixelformat(this); }
	void s_pixelformat(__u32 pixelformat) { v4l_format_s_pixelformat(this, pixelformat); }
	unsigned g_colorspace() { return v4l_format_g_colorspace(this); }
	void s_colorspace(unsigned colorspace) { v4l_format_s_colorspace(this, colorspace); }
	__u8 g_num_planes() { return v4l_format_g_num_planes(this); }
	void s_num_planes(__u8 num_planes) { v4l_format_s_num_planes(this, num_planes); }
	__u32 g_bytesperline(unsigned plane = 0) { return v4l_format_g_bytesperline(this, plane); }
	void s_bytesperline(__u32 bytesperline, unsigned plane = 0) { v4l_format_s_bytesperline(this, bytesperline, plane); }
	__u32 g_sizeimage(unsigned plane = 0) { return v4l_format_g_sizeimage(this, plane); }
	void s_sizeimage(__u32 sizeimage, unsigned plane = 0) { v4l_format_s_sizeimage(this, sizeimage, plane); }
	unsigned g_field() { return v4l_format_g_field(this); }
	void s_field(unsigned field) { v4l_format_s_field(this, field); }
	unsigned g_first_field(v4l2_std_id std) { return v4l_format_g_first_field(this, std); }
	unsigned g_flds_per_frm() { return v4l_format_g_flds_per_frm(this); }
};

class cv4l_buffer;

class cv4l_queue : v4l_queue {
	friend class cv4l_buffer;
public:
	cv4l_queue(unsigned type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
		   unsigned memory = V4L2_MEMORY_MMAP)
	{
		v4l_queue_init(this, type, memory);
	}
	void init(unsigned type, unsigned memory)
	{
		v4l_queue_init(this, type, memory);
	}
	unsigned g_type() const { return v4l_queue_g_type(this); }
	unsigned g_memory() const { return v4l_queue_g_memory(this); }
	unsigned g_buffers() const { return v4l_queue_g_buffers(this); }
	unsigned g_num_planes() const { return v4l_queue_g_num_planes(this); }
	unsigned g_length(unsigned plane) const { return v4l_queue_g_length(this, plane); }
	unsigned g_mem_offset(unsigned index, unsigned plane) const { return v4l_queue_g_mem_offset(this, index, plane); }
	void *g_mmapping(unsigned index, unsigned plane) const { return v4l_queue_g_mmapping(this, index, plane); }
	void s_mmapping(unsigned index, unsigned plane, void *m) { v4l_queue_s_mmapping(this, index, plane, m); }
	void *g_userptr(unsigned index, unsigned plane) const { return v4l_queue_g_userptr(this, index, plane); }
	void s_userptr(unsigned index, unsigned plane, void *m) { v4l_queue_s_userptr(this, index, plane, m); }
	void *g_dataptr(unsigned index, unsigned plane) const { return v4l_queue_g_dataptr(this, index, plane); }
	int g_fd(unsigned index, unsigned plane) const { return v4l_queue_g_fd(this, index, plane); }
	void s_fd(unsigned index, unsigned plane, int fd) { v4l_queue_s_fd(this, index, plane, fd); }

	int reqbufs(cv4l_fd *fd, unsigned count = 0)
	{
		return v4l_queue_reqbufs(fd->g_v4l_fd(), this, count);
	}
	bool has_create_bufs(cv4l_fd *fd) const
	{
		return v4l_queue_has_create_bufs(fd->g_v4l_fd(), this);
	}
	int create_bufs(cv4l_fd *fd, unsigned count, const v4l2_format *fmt = NULL)
	{
		return v4l_queue_create_bufs(fd->g_v4l_fd(), this, count, fmt);
	}
	int mmap_bufs(cv4l_fd *fd, unsigned from = 0)
	{
		return v4l_queue_mmap_bufs(fd->g_v4l_fd(), this, from);
	}
	int munmap_bufs(cv4l_fd *fd)
	{
		return v4l_queue_munmap_bufs(fd->g_v4l_fd(), this);
	}
	int alloc_bufs(cv4l_fd *fd, unsigned from = 0)
	{
		return v4l_queue_alloc_bufs(fd->g_v4l_fd(), this, from);
	}
	int free_bufs()
	{
		return v4l_queue_free_bufs(this);
	}
	int obtain_bufs(cv4l_fd *fd, unsigned from = 0)
	{
		return v4l_queue_obtain_bufs(fd->g_v4l_fd(), this, from);
	}
	int release_bufs(cv4l_fd *fd)
	{
		return v4l_queue_release_bufs(fd->g_v4l_fd(), this);
	}
	bool has_expbuf(cv4l_fd *fd)
	{
		return v4l_queue_has_expbuf(fd->g_v4l_fd());
	}
	int export_bufs(cv4l_fd *fd)
	{
		return v4l_queue_export_bufs(fd->g_v4l_fd(), this);
	}
	void close_exported_fds()
	{
		v4l_queue_close_exported_fds(this);
	}
	void free(cv4l_fd *fd)
	{
		v4l_queue_free(fd->g_v4l_fd(), this);
	}
	void buffer_init(v4l_buffer &buf, unsigned index) const
	{
		v4l_queue_buffer_init(this, &buf, index);
	}
	int queue_all(cv4l_fd *fd);
};

class cv4l_buffer : public v4l_buffer {
public:
	cv4l_buffer(unsigned type = 0, unsigned memory = 0, unsigned index = 0)
	{
		init(type, memory, index);
	}
	cv4l_buffer(const cv4l_queue &q, unsigned index = 0)
	{
		init(q, index);
	}
	cv4l_buffer(const cv4l_buffer &b)
	{
		init(b);
	}
	virtual ~cv4l_buffer() {}

	void init(unsigned type = 0, unsigned memory = 0, unsigned index = 0)
	{
		v4l_buffer_init(this, type, memory, index);
	}
	void init(const cv4l_queue &q, unsigned index = 0)
	{
		q.buffer_init(*this, index);
	}
	void init(const cv4l_buffer &b)
	{
		*this = b;
		if (v4l_type_is_planar(g_type()))
			buf.m.planes = planes;
	}

	__u32 g_index() const { return v4l_buffer_g_index(this); }
	void s_index(unsigned index) { v4l_buffer_s_index(this, index); }
	unsigned g_type() const { return v4l_buffer_g_type(this); }
	unsigned g_memory() const { return v4l_buffer_g_memory(this); }
	__u32 g_flags() const { return v4l_buffer_g_flags(this); }
	void s_flags(__u32 flags) { v4l_buffer_s_flags(this, flags); }
	void or_flags(__u32 flags) { v4l_buffer_or_flags(this, flags); }
	unsigned g_field() const { return v4l_buffer_g_field(this); }
	void s_field(unsigned field) { v4l_buffer_s_field(this, field); }

	unsigned g_num_planes() const { return v4l_buffer_g_num_planes(this); }
	__u32 g_mem_offset(unsigned plane = 0) const { return v4l_buffer_g_mem_offset(this, plane); }
	void *g_userptr(unsigned plane = 0) const { return v4l_buffer_g_userptr(this, plane); }
	void s_userptr(void *userptr, unsigned plane = 0) { v4l_buffer_s_userptr(this, plane, userptr); }
	int g_fd(unsigned plane = 0) const { return v4l_buffer_g_fd(this, plane); }
	void s_fd(int fd, unsigned plane = 0) { v4l_buffer_s_fd(this, plane, fd); }
	__u32 g_bytesused(unsigned plane = 0) const { return v4l_buffer_g_bytesused(this, plane); }
	void s_bytesused(__u32 bytesused, unsigned plane = 0) { v4l_buffer_s_bytesused(this, plane, bytesused); }
	__u32 g_data_offset(unsigned plane = 0) const { return v4l_buffer_g_data_offset(this, plane); }
	void s_data_offset(__u32 data_offset, unsigned plane = 0) { v4l_buffer_s_data_offset(this, plane, data_offset); }
	__u32 g_length(unsigned plane = 0) const { return v4l_buffer_g_length(this, plane); }
	void s_length(unsigned length, unsigned plane = 0) { return v4l_buffer_s_length(this, plane, length); }

	__u32 g_sequence() const { return v4l_buffer_g_sequence(this); }
	__u32 g_timestamp_type() const { return v4l_buffer_g_timestamp_type(this); }
	__u32 g_timestamp_src() const { return v4l_buffer_g_timestamp_src(this); }
	void s_timestamp_src(__u32 src) { v4l_buffer_s_timestamp_src(this, src); }
	bool ts_is_copy() const { return v4l_buffer_is_copy(this); }
	const timeval &g_timestamp() const { return *v4l_buffer_g_timestamp(this); }
	void s_timestamp(const timeval &tv) { v4l_buffer_s_timestamp(this, &tv); }
	void s_timestamp_ts(const timespec &ts) { v4l_buffer_s_timestamp_ts(this, &ts); }
	void s_timestamp_clock() { v4l_buffer_s_timestamp_clock(this); }
	const v4l2_timecode &g_timecode() const { return *v4l_buffer_g_timecode(this); }
	void s_timecode(const v4l2_timecode &tc) { v4l_buffer_s_timecode(this, &tc); }
};

inline int cv4l_queue::queue_all(cv4l_fd *fd)
{
	cv4l_buffer buf;

	for (unsigned i = 0; i < g_buffers(); i++) {
		buf.init(*this, i);
		int ret = fd->qbuf(buf);
		if (ret)
			return ret;
	}
	return 0;
}

#endif
