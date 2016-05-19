#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> //borrar
#define TAMANO_INICIAL 20

typedef struct heap
{	void** datos;
	size_t cantidad_elementos;
	size_t capacidad;
	cmp_func_t comparar_prioridad;
} heap_t;

size_t redondear_div(size_t dividendo, size_t divisor)
{
    return (dividendo + (divisor / 2)) / divisor;
}
heap_t *heap_crear(cmp_func_t cmp)
{
	heap_t *heap_nuevo = malloc(sizeof(heap_t));
	void** datos_genericos = malloc(sizeof(void*)*TAMANO_INICIAL);
	if (!heap_nuevo || !datos_genericos)
		{return NULL;}
	heap_nuevo->datos = datos_genericos;
	heap_nuevo->capacidad = TAMANO_INICIAL;
	heap_nuevo->cantidad_elementos = 0;
	heap_nuevo->comparar_prioridad = cmp;
	return heap_nuevo;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e))
{	if (destruir_elemento)
	{	
		size_t limite = heap->cantidad_elementos;    
		for (size_t i = 0; i < limite; i++)
			{destruir_elemento(heap->datos[i]);}
	}
	free(heap->datos);
	free(heap);
}

bool heap_esta_vacio(const heap_t *heap)
	{return !heap->cantidad_elementos;}

size_t heap_cantidad(const heap_t *heap)
	{return heap->cantidad_elementos;}
											
void upheap(heap_t* heap,size_t elemento_actual)
{
	if (elemento_actual == 0)
		{return;} //Me encuentro en la raiz.
	size_t padre_actual = redondear_div(elemento_actual-2,2);
	
	if (heap->comparar_prioridad(heap->datos[elemento_actual],heap->datos[padre_actual]) > 0) // el padre es menor!
	{
		
		void* aux;
		aux = heap->datos[padre_actual];
		heap->datos[padre_actual] = heap->datos[elemento_actual];
		heap->datos[elemento_actual] = aux;
		return upheap(heap,padre_actual);
	}
}
bool heap_encolar(heap_t *heap, void *elem) 
{
	heap->datos[heap->cantidad_elementos] = elem; 
	upheap(heap,heap->cantidad_elementos);
	heap->cantidad_elementos++;
	return true;
}

void *heap_ver_max(const heap_t *heap)
{
	if (heap_esta_vacio(heap))
		{return NULL;}
	return heap->datos[0];
}



void downheap(heap_t* heap,size_t elemento_actual){
	if(elemento_actual == heap->cantidado_elementos-1){
		return;
	}
	size_t hijo_der = elemento_actual * 2 + 1;
	size_t hijo_izq = elemento_actual * 2 + 2;
	size_t mayor; 
	if(heap->comparar_prioridad(heap->datos[elemento_actual], heap->datos[hijo_izq]) < 0){
		if((heap->comparar_prioridad(heap->datos[elemento_actual], heap->datos[hijo_der]) < 0) && (heap_comparar_prioridad(heap->datos[hijo_der], heap->datos[hijo_izq]) > 0)){
			mayor = hijo_der;
		}else{
			mayor = hijo_izq;
		}
	}else if(heap->comparar_prioridad(heap->datos[elemento_actual], heap->datos[hijo_der]) < 0){
		mayor = hijo_der;
	}else{
		return;
	}
	void *aux = heap->datos[elemento_actual];
	heap->datos[elemento_actual] = heap->datos[mayor];
	heap->datos[mayor] = aux;
	return downheap(heap, mayor);
	
}

void downheap(void **datos,cmp_func_t comparar_prioridad,size_t cantidad_elementos, size_t posicion_actual)
{	size_t pos_h_izquierdo = (2*posicion_actual)+1;
	size_t pos_h_derecho = (2*posicion_actual)+2;
    if (pos_h_izquierdo >= cantidad_elementos) //Arbol izquierdista
       {return;}
	else if (pos_h_derecho >= cantidad_elementos) //Solo un hijo izquierdo
	{
		if (comparar_prioridad(datos[posicion_actual],datos[pos_h_izquierdo]) == -1) 
		{
			void* aux = datos[posicion_actual];
			datos[posicion_actual] =  datos[pos_h_izquierdo];
			datos[pos_h_izquierdo] = aux;
			downheap(datos,comparar_prioridad,cantidad_elementos,pos_h_izquierdo);
		}
	}
	else //Hay dos hijos
	{
		if ((comparar_prioridad(datos[posicion_actual],datos[pos_h_derecho])) > 0)
			{
				if ((comparar_prioridad(datos[posicion_actual],datos[pos_h_izquierdo])) > 0)
				{return;}
			}
		size_t pos_prioritaria = pos_h_izquierdo;
		if ((comparar_prioridad(datos[pos_h_izquierdo],datos[pos_h_derecho]) < 0))
		    {pos_prioritaria = pos_h_derecho;}
		void* aux = datos[posicion_actual];
		datos[posicion_actual] = datos[pos_prioritaria];
		datos[pos_prioritaria] = aux;	
		downheap(datos,comparar_prioridad,cantidad_elementos,pos_prioritaria);
	}
}

void *heap_desencolar(heap_t *heap)
{
	if (heap_esta_vacio(heap))
		{return NULL;}
	void *primero = heap->datos[0];
	heap->datos[0] = heap->datos[heap->cantidad_elementos-1];
	heap->cantidad_elementos--;
	downheap(heap->datos,heap->comparar_prioridad,heap->cantidad_elementos,0);
	if (heap->capacidad != TAMANO_INICIAL)
	{
		size_t capacidad = heap->capacidad;
		size_t cantidad = heap->cantidad_elementos;
		size_t resultado = redondear_div(capacidad,4);
		if (cantidad <= resultado)
			{heap_redimensionar(heap,capacidad/DOBLE);}
	}
	return primero;
}

void heapify(void* arreglo[],size_t cantidad_elementos,cmp_func_t comparar_prioridad )
{
	int inicio = (int)cantidad_elementos;
	for (int i = inicio-1; i >= 0; i--)
		{downheap(arreglo,comparar_prioridad,cantidad_elementos,i);}
