# population.R
# R wrapper for a PopJSON-compiled shared library

PopulationModel <- setRefClass("PopulationModel",
  fields = list(
    lib = "character",
    dll = "ANY",
    numpop = "integer",
    numpar = "integer",
    numint = "integer",
    numenv = "integer",
    stoch = "integer",
    param = "numeric",
    parmin = "numeric",
    parmax = "numeric",
    popnames = "character",
    parnames = "character",
    intnames = "character",
    envnames = "character",
    popids = "list",
    parids = "list",
    intids = "list",
    envids = "list"
  ),

  methods = list(

    initialize = function(libpath) {
      lib <<- libpath
      dll <<- dyn.load(lib)

      res <- .C("init",
                no = as.integer(0),
                np = as.integer(0),
                ni = as.integer(0),
                ne = as.integer(0),
                st = as.integer(0))

      numpop <<- res$no
      numpar <<- res$np
      numint <<- res$ni
      numenv <<- res$ne
      stoch  <<- res$st

      total <- numpop + numpar + numint + numenv
      namebuf <- rep("", total)
      param_buf <- numeric(numpar)
      parmin_buf <- numeric(numpar)
      parmax_buf <- numeric(numpar)

      res_names <- .C("parnames",
                      names = as.character(namebuf),
                      param = as.double(param_buf),
                      parmin = as.double(parmin_buf),
                      parmax = as.double(parmax_buf))

      allnames <- res_names$names
      popnames <<- allnames[1:numpop]
      parnames <<- allnames[(numpop + 1):(numpop + numpar)]
      intnames <<- allnames[(numpop + numpar + 1):(numpop + numpar + numint)]
      envnames <<- allnames[(numpop + numpar + numint + 1):total]

      param <<- res_names$param
      parmin <<- res_names$parmin
      parmax <<- res_names$parmax

      # ID lookup lists (R is 1-based)
      popids <<- setNames(as.list(seq_along(popnames)), popnames)
      parids <<- setNames(as.list(seq_along(parnames)), parnames)
      intids <<- setNames(as.list(seq_along(intnames)), intnames)
      envids <<- setNames(as.list(seq_along(envnames)), envnames)

      message("Loaded model with: ",
              numpop, " pops, ",
              numpar, " pars, ",
              numint, " ints, ",
              numenv, " envs.")
    },

    define_function = function(name, numpar) {
      fname <- paste0("define_", name)
      tryCatch({
        function(...) {
          args <- list(...)
          stopifnot(length(args) == numpar)
          inputs <- lapply(args, as.double)
          all_args <- c(inputs, list(result = as.double(0)))
          result <- do.call(".C", c(list(fname, PACKAGE = basename(lib)), all_args))
          result$result
        }
      }, error = function(e) {
        warning(sprintf("Function '%s' not found in shared object.", fname))
        return(NULL)
      })
    },

    sim = function(ftime, envir = numeric(0), pr = numeric(0), y0 = numeric(0), rep = 1, file0 = "", file1 = "", boil = FALSE) {
      ftime <- as.integer(ftime)
      rep <- as.integer(rep)
      file0 <- as.character(file0)
      file1 <- as.character(file1)

      ret <- double(ftime * numpop)
      iret <- double((ftime - 1) * numint)
      success <- as.integer(0)

      .C("sim",
         tf = ftime,
         rep = rep,
         envir = as.double(envir),
         pr = as.double(pr),
         y0 = as.double(y0),
         file0 = file0,
         file1 = file1,
         ret = ret,
         iret = iret,
         success = success)

      list(
        success = success,
        ret = matrix(ret, nrow = ftime, byrow = TRUE),
        iret = matrix(iret, nrow = ftime - 1, byrow = TRUE)
      )
    },

    destroy = function() {
      dyn.unload(lib)
    }
  )
)
