
/******************************************************
 * Cache is a standard component for defining feedback loops
 *  This prevents multiple evaluations.
 ******************************************************/

Component *cache(Source *s)
{
  Component *c = new Cache();
  c->attach("input", s);
  return c;
}
// The coomponent version of the class Cache
