

#include "kfifo_DMA.h"
#include <assert.h>
#include <string.h>
#include "main.h"

#ifdef KFIFO_DYNAMIC_ALLOCATE
#include "FreeRTOS.h"
#include "task.h"
#endif
#define min(a, b)                (((a) < (b)) ? (a) : (b))



// 判断是不是完全平方数
bool is_pow_of_2(uint32_t n )
{

	int idxZ1 = -1;
	for(int i = 0; i < sizeof(n) * 8; ++i)
	{
		if((n & 0x01U) == 1U )
		{
			if(idxZ1 >= 0) // found more than one 1s from LSB
			{
				return false;
			}
			else // found first 1 from LSB
			{
				idxZ1 = i;
			}
		}
		n >>= 1;
	}
	return true;

}


void kfifo_DMA_static_init(KFIFO_DMA* p, uint8_t* buf, size_t size, size_t dmastep)
{

	p->in = 0;
	p->out = 0;
	p->size = size;
	p->half = size >> 1;
	p->buffer = buf;
	p->bytesWritten = 0;
	p->bytesRead = 0;
	uint8_t leading_zeros = __CLZ(size);
	p->mask_size = ((uint32_t)0xffffffffU) >> (leading_zeros+1);
#ifdef USE_DMA_ON_KFIFO
	p->pow_of_two = (32U - __clz(size) - 1);
	p->half = size >> 1;
	p->dmacur = 0;
	p->dmastep = dmastep;
#endif
}


//static inline uint32_t __kfifo_wrap_inc(uint32_t a, uint32_t b, uint32_t total_len)
//{
//	uint32_t s = a + b;
//	if(s >= total_len) s -= total_len;
//	return s;
//}

static inline uint32_t __kfifo_mod(struct KFIFO_DMA *fifo, uint32_t a)
{


	return a & fifo->mask_size;
}


size_t __kfifo_put(struct KFIFO_DMA *fifo, const uint8_t *buffer, size_t len)
{
	if(len == 0)
		return 0;

        //环形缓冲区的剩余容量为fifo->size - fifo->in + fifo->out，让写入的长度取len和剩余容量中较小的，避免写越界；
    // NOTE: in - out can in no case be greater than size, never, the code prevents that from happening.
    // so do not think about that.
    size_t freespace = kfifo_get_free_space(fifo);
    if(len > freespace)
    {
//    	fifo->out += (len - freespace);
    	return 0;
    }
    uint32_t in_mod = __kfifo_mod(fifo, fifo->in);
	fifo->in += len;
/*
     * Ensure that we sample the fifo->out index -before- we
     * start putting bytes into the kfifo.
     */
	// for multicore
	//    smp_mb();

    size_t L;

    /* first put the data starting from fifo->in to buffer end */
    L = min(len, fifo->size - in_mod);
    memcpy(fifo->buffer + in_mod, buffer, L);

    /* then put the rest (if any) at the beginning of the buffer */
    if(len - L)
    	memcpy(fifo->buffer, buffer + L, len - L);

    /*
     * Ensure that we add the bytes to the kfifo -before-
     * we update the fifo->in index.
     */

  // smp_wmb();   //for multicore


    // override

    fifo->bytesWritten += len;
    // return freespace to help the caller to judge if override occures (when freespace < len)
    return freespace;
}


size_t __kfifo_get(struct KFIFO_DMA *fifo, uint8_t *buffer, size_t len)
{
	if(len == 0)
		return 0;
    unsigned int L;

    // NOTE: in - out can in no case be greater than size, never, the code prevents that from happening.
    // so do not think about that.
    len = min(len, kfifo_get_filled_size(fifo));
    uint32_t out_mod = __kfifo_mod(fifo, fifo->out);
    /*
     * Ensure that we sample the fifo->in index -before- we
     * start removing bytes from the kfifo.
     */

    /* first get the data from fifo->out until the end of the buffer */
    L = min(len, fifo->size - out_mod );
    memcpy(buffer, fifo->buffer + out_mod, L);

    /* then get the rest (if any) from the beginning of the buffer */
    if(len - L)
    	memcpy(buffer + L, fifo->buffer, len - L);


    fifo->out += len;
    fifo->bytesRead+=len;
    return len;
}

void kfifo_invalidate(KFIFO_DMA* fifo)
{
	fifo->out = fifo->in;
}

size_t kfifo_get_free_space( KFIFO_DMA *fifo)
{
	return fifo->size - kfifo_get_filled_size(fifo);
}

size_t kfifo_get_filled_size( KFIFO_DMA *fifo)
{
	return fifo->in - fifo->out;
}



#ifdef USE_DMA_ON_KFIFO
int kfifo_DMA_HalfCplt_cb(KFIFO_DMA* p,)
{
	int r = KFIFO_DMA_OK;
	p->dmacur += (p->dmastep >> 1);
	// CASE: not underrun
	if(p->dmacur < p->in)
	{
		p->out = p->dmacur;
	}
	// CASE: FIFO underrun - out of new data to transmit
	else
	{
		p->out = p->in; // empty the buffer
		//notify the caller to stop the DMA
		r = KFIFO_DMA_UNDERRUN;
	}
	return r;
}

#endif
