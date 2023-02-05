# https://shinyapps.ecdc.europa.eu/shiny/AedesRisk/

funPrec <- function(precipitations) {
  Precipitationcumdays <- 14  # Precipitation cumulative period in days
  timePrec <- 1:length(precipitations)
  return(sapply(timePrec, function(tt) {
      id1 <- which(timePrec >= (tt - Precipitationcumdays + 1))[1]
      id2 <- which(timePrec >= tt)[1]
      if (is.na(id1) | is.na(id2)) {
        return(0)
      }
      return(sum(precipitations[(id1:id2)]))
  }))
}

dxdt <- function(tm, vec, params) {
  temperature <- params$temperature
  precipitations <- params$precipitations
  precipiNorm <- params$precipiNorm
  Kcap <- params$optim

  # Tran et al. 2013 (Table 2)
  epsilon1 <- 95     # Egg production primigravidae
  epsilon2 <- 75     # Egg production parous
  muE      <- 0.01 #5   # Egg mortality
  muL      <- 0.08   # Larva mortality
  muP      <- 0.03   # Pupa mortality
  sigma    <- 0.5    # Sex ratio
  alphaA   <- 0.4    # Development rate
  alphaH   <- 0.222 #0.2    # Transition rate
  alphaAh  <- 0.222 #0.2    # Transition rate from host-seeking to engorged adults
  muAm     <- 0.1    # Mortality rate
  muT      <- 0.08   # Adult mortality rate related to seeking behavior
  muA      <- 0.02   # Minimum adult mortality rate
  KL       <- Kcap #1e6#250000 # Carrying capacity for larvae
  KP       <- Kcap #1e6#250000 # Carrying capacity for pupae
  tstart   <- 69     # Start of the favourable season (10 Mar)
  tend     <- 273    # End of the favourable season (30 Sept)

  lyear    <- 365

  # AedesRisk
  PrecipitationFactor  <- 1   # Precipitation effect multiplier
  # Precipitationcumdays <- 14  # Precipitation cumulative period in days
  tendspring           <- 166 # End of the spring during which eggs only hatch on days with 1mm or more rain
  tstartautumn         <- 258 # Start of the autumn during which eggs only hatch on days with 1mm or more rain

  #Current state
  Ei   <- vec[1]
  Li   <- vec[2]
  Pi   <- vec[3]
  Ami  <- vec[4]
  Smi1 <- vec[5]
  Egi1 <- vec[6]
  Ogi1 <- vec[7]
  Smi2 <- vec[8]
  Egi2 <- vec[9]
  Ogi2 <- vec[10]

  #Calculating event rates
  # rates <- matrix(1,1,22)

  #Getting temperature
  timeTemp <- 1:length(temperature)
  id <- which(timeTemp >= tm)[1]
  if (is.na(id)) {
    return(0)
  }
  currentTemp <- temperature[id]

  #Normalized precipitation during the last two weeks
#   timePrec <- 1:length(precipitations)
#   id1 <- which(timePrec >= (tm - Precipitationcumdays + 1))[1]
#   id2 <- which(timePrec >= tm)[1]
#   if (is.na(id1) | is.na(id2)) {
#     return(0)
#   }
#   currentPrec <- sum(precipitations[(id1:id2)])

  timePrec <- 1:length(precipitations)
  id2 <- which(timePrec >= tm)[1]
  currentPrec <- precipiNorm[id2]
  currentPrecNorm <- currentPrec/max(precipiNorm,na.rm=TRUE)

  ##### TRANSITION STAGES ####
  #Transition from egg to larvae
  alphaE <- (currentTemp - 10.4)/110
  if (alphaE < 0) {alphaE <- 0}

  #### NO HATCHING IN SPRING OR AUTUMN WITH <1mm RAIN ON THE DAY ####
  if ((tm%%lyear < tendspring | tm%%lyear > tstartautumn) & 
      (round(precipitations[id2], digits=2) < 1)) {alphaE <- 0}

  #Transition from larvae to pupae
  alphaL <- -0.0007*(currentTemp^2) + 0.0392*currentTemp - 0.3911
  if (alphaL < 0) {alphaL <- 0}

  #Transition from puppae to adults
  alphaP <- 0.0008*(currentTemp^2) - 0.0051*currentTemp + 0.0319
  if (alphaP < 0) {alphaP <- 0}

  ##########################################################################
        
  #Transition from engorged to ovipositing
  alphaO <- (currentTemp - 10)/77
  if (alphaO < 0) {alphaO <- 0}

  #### DIAPAUSE ####
  z <- if (tm%%lyear >= tstart & tm%%lyear < tend) {1} else {0}

  ##################
  #### MORTALITY ####
  #Mortality larvae
  muLCurrent <- exp(-currentTemp/2) + muL
  #Mortality pupae
  muPCurrent <- exp(-currentTemp/2) + muP
  #Mortality adult
  muAmCurrent <- max(c(muA, 0.04417 + 0.00217*currentTemp), na.rm=TRUE)

  #### CARRYING CAPACITY ####
  #Carrying capacity of larval stage
  # KLCurrent <- KL*(PrecipitationFactor*(currentPrec/100) + 1) #Original Tran et al. Note that the maximum daily rainfall in Nice was 99.8 (close to 100), but more when aggregated over a 14 day period.
  KLCurrent <- KL*(PrecipitationFactor*(currentPrecNorm)+1)

  #Carrying capacity of pupae stage
  # KPCurrent <- KP*(PrecipitationFactor*(currentPrec/100) + 1)
  KPCurrent <- KP*(PrecipitationFactor*(currentPrecNorm)+1)
        
  dEi   <- alphaH*(epsilon1*Ogi1 + epsilon2*Ogi2) - (muE + z*alphaE)*Ei                  # dE
  dLi   <- z*alphaE*Ei - (muLCurrent*(1 + (Li/KLCurrent)) + alphaL)*Li                   # dL
  dPi   <- alphaL*Li - (muPCurrent + alphaP)*Pi                                          # dP
  dAmi  <- alphaP*Pi*sigma*exp(-muAm*(1 + (Pi/KPCurrent))) - (muAmCurrent + alphaA)*Ami  # dA_{em}
  dSmi1 <- alphaA*Ami - (muAmCurrent + muT + alphaAh)*Smi1                               # dA_{1h}
  dEgi1 <- alphaAh*Smi1 - (muAmCurrent + alphaO)*Egi1                                    # dA_{1g}
  dOgi1 <- alphaO*Egi1 - (muAmCurrent + muT + alphaH)*Ogi1                               # dA_{1o}
  dSmi2 <- alphaH*(Ogi1 + Ogi2) - (muAmCurrent + muT + alphaAh)*Smi2                     # dA_{2h}
  dEgi2 <- alphaAh*Smi2 - (muAmCurrent + alphaO)*Egi2                                    # dA_{2g}
  dOgi2 <- alphaO*Egi2 - (muAmCurrent + muT + alphaH)*Ogi2                               # dA_{2o}

  return(list(c(
    dEi,
    dLi,
    dPi,
    dAmi,
    dSmi1,
    dEgi1,
    dOgi1,
    dSmi2,
    dEgi2,
    dOgi2
  )))
}
